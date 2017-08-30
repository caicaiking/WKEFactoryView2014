#include "clsMyserialPort.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
#include <QDebug>
#include "clsSelectComport.h"
#include <QApplication>
clsMyserialPort::clsMyserialPort(QString comNumber)
{
    this->comNumber = comNumber;
    initSerial();
}

clsMyserialPort::clsMyserialPort()
{
    this->comNumber = checkPorts();
    initSerial();
}

clsMyserialPort::~clsMyserialPort()
{
    if(blInit && serialPort !=NULL)
    {
        serialPort->close();
        blInit = false;
    }
    qDebug()<<"Close serial Port";
}


void clsMyserialPort::initSerial()
{
    if(this->comNumber.isEmpty())
    {
        blInit= false;
        qDebug()<<"No com port to use";
        return;
    }

    serialPort = new QSerialPort(this->comNumber);

    serialPort->setBaudRate( QSerialPort::Baud9600);            //9600
    serialPort->setParity(QSerialPort::NoParity);               //N
    serialPort->setDataBits(QSerialPort::QSerialPort::Data8);   //8
    serialPort->setStopBits(QSerialPort::OneStop);              //1
    serialPort->setFlowControl(QSerialPort::HardwareControl);

    if(serialPort->open( QIODevice::ReadWrite))
    {
        blInit = true;
        qDebug()<<"Serial port opened!";
    }
    else
    {
        blInit = false;
        qDebug()<<"Serial port open failed!";
    }
}

QString clsMyserialPort::checkPorts()
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

bool clsMyserialPort::getBlInit() const
{
    return blInit;
}

void clsMyserialPort::setCom(QString value)
{
    this->comNumber = value;
}

QString clsMyserialPort::sendCommand(QString cmmd, bool hasReturn)
{
    if(!blInit)
        return "";

    QString cmd = cmmd.append("\n");
    serialPort->write(cmd.toStdString().c_str(), cmd.length());
    serialPort->waitForBytesWritten(5000);

    if(cmd.contains("OUTP"))
        sleepMs(1000);

    if(!hasReturn)
        return "";

    serialPort->waitForReadyRead(5000);
    QString readValue =serialPort->readLine();
    readValue = readValue.remove("\n"); //去掉结尾的回车符号
    return readValue;
}

void clsMyserialPort::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void clsMyserialPort::closePort()
{
    if(blInit && serialPort !=NULL)
    {
        serialPort->close();
        blInit=false;
    }
}


