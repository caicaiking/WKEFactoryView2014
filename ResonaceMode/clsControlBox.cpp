#include "clsControlBox.h"
#include <QObject>
#include <QApplication>
#include <QTime>
#include <QDebug>
clsControlBox::clsControlBox(QObject *parent) :
    QObject(parent)
{
    this->blStop=false;
    isInit = this->initDevice();
}

QString clsControlBox::sendCommand(QString value, bool hasReturn)
{
    char sBuffer[8];

    WriteAduDevice(hDevice, value.toStdString().c_str(), 4, 0, 0);
    memset(sBuffer, 0, sizeof(sBuffer));
    if(!hasReturn)
        return "";

    ReadAduDevice(hDevice, sBuffer, 7, 0, 0);
    QString ret = sBuffer;
    return ret;
}

QString clsControlBox::getInput(int port)
{
    QString cmmd = QString("RPA%1").arg(QString::number(port));
    return sendCommand(cmmd,true);
}

bool clsControlBox::getInputSignal(int port)
{
    blStop=false;

    int status=getInput(port).toInt();
    if(status !=0)
    {
        while((getInput(port)!="0") && (!blStop))
        {
            qApp->processEvents();
            sleepMs(10);
        }
     //   qDebug()<<"1";
    }
   // qDebug()<<"11";

    if(blStop)
        return false;

    //0->1
    while((getInput(port)=="0") && (!blStop))
    {
        qApp->processEvents();
        sleepMs(10);
    }
    if(blStop)
        return false;
    //1->0
    while((getInput(port)=="1") && (!blStop))
    {
        qApp->processEvents();
        sleepMs(10);
    }

    if(blStop)
        return false;
   // qDebug()<<"2";
    sleepMs(20);
    status=getInput(port).toInt();

    if(status==0)
        return true;
    else
        return false;

}

void clsControlBox::stop()
{
    this->blStop=true;
}

bool clsControlBox::initDevice()
{
    hDevice = OpenAduDevice(0);
    int i=(int)hDevice;
    if(i !=-1 )
        return true;
    return false;
}

clsControlBox::~clsControlBox()
{
    CloseAduDevice(hDevice);
}
void clsControlBox::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

