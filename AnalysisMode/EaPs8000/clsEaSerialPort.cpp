#include "clsEaSerialPort.h"
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "clsSelectComport.h"
clsEaSerialPort::clsEaSerialPort(QObject *parent) : QObject(parent)
{
    blInit = false;
}

bool clsEaSerialPort::openSerialPort()
{
    //
    QString portName =checkPorts();
    myCom = new QextSerialPort(portName);
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));

    myCom->setBaudRate(BAUD57600);
    myCom->setDataBits(DATA_8);
    myCom->setParity(PAR_ODD);
    myCom->setStopBits(STOP_1);
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(200);

    //以可读写方式打开串口
    blInit = myCom->open(QIODevice::ReadWrite);

    if(false == blInit)
    {
        blInit=false;
        QMessageBox::warning(0,tr("Warning"),tr("串口不存在或已被占用!"),QMessageBox::Yes);
        return false;
    }

    return blInit;
}

bool clsEaSerialPort::closeSerialPort()
{
    if(blInit)
    {
        myCom->close();
        blInit = false;
    }
}

QByteArray clsEaSerialPort::sendCommand(QByteArray cmd)
{

    res.clear(); //清除数据

    if(blInit == false)
        return res;

    if(!(myCom->isOpen())) return res;

    if(cmd.isEmpty()) return res;

    myCom->write(cmd);

    blStop = false;
    QTimer::singleShot(200, this, SLOT(setStop()));

    sleepMs(50);
    while(res.length() == 0  && (blStop == false))
    {
      qApp->processEvents(QEventLoop::AllEvents, 100);
    }

    return res;
}

clsEaSerialPort::~clsEaSerialPort()
{
    if(blInit)
    {
        myCom->close();
        blInit = false;
    }
}

void clsEaSerialPort::readMyCom()
{
   res = myCom->readAll();
}

void clsEaSerialPort::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void clsEaSerialPort::setStop()
{
    blStop = true;
}

QString clsEaSerialPort::checkPorts()
{
    QList<QSerialPortInfo> avPort = QSerialPortInfo::availablePorts();
    QList<QString> ports;
    foreach (QSerialPortInfo tmp, avPort) {
        ports<<tmp.portName();
    }

    if(ports.length()>1)
    {
        clsSelectComport st;
        st.setComports(ports);

        if(st.exec() == QDialog::Accepted)
        {
            return st.getPortSelected();
        }
        else
        {
            return "";
        }
    }
    else if(ports.length() ==1)
        return ports.first();
    else
        return "";
}
