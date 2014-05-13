#include "clsHVRS232.h"
#include <QtCore>
#include <QObject>
#define ACK 6
#define NAK 21
#include <stdlib.h>

clsHVRs232::clsHVRs232()
{
    isInit=false;
    timer.setInterval(3000);
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(timeOut()));

}

bool clsHVRs232::init()
{
    return isInit;
}

void clsHVRs232::setAddress(QString portName)
{

    serial = new clsPortListener(portName);

    isInit = serial->getInit();


    QObject::connect(serial,SIGNAL(reciveStr(QByteArray)),this,SLOT(readData(QByteArray)),Qt::DirectConnection);
}

QString clsHVRs232::sendCommand(QString strCmmd, bool hasReturn )
{
    serial->sendCommand(strCmmd);

    isFilled=false;
    strRecive ="";

    isTimeOut = false;
    timer.start();

    if(!hasReturn)
        return "";

    //to here, hv will automatic send the data back.
    while(!isFilled && ! isTimeOut)
    {
        qApp->processEvents();
    }

    if(!isTimeOut)
    {
        timer.stop();
        isFilled=false;
        QString ret =strRecive;
        strRecive ="";
        return ret;
    }
    else
    {
        emit errorMsg(tr("Send command to HV with Rs232 time out"));
        isFilled=false;
        strRecive="";
        return "";
    }
}

void clsHVRs232::setCurVoltage(double  value)
{
    NumberType number;
    number.setValue(QString::number(value));

    if(!init())
        return;

    if(this->meter.curVoltage.PointValue != number.PointValue)
    {
        sendCommand("KF",false);
        sleepMs(100);

        while(std::abs(this->meter.curVoltage.PointValue-number.PointValue)>10)
        {
            if(this->meter.curVoltage.PointValue < number.PointValue)
            {
                sendCommand("SV+",false);
            }
            else
            {
                sendCommand("SV-",false);
            }

            sleepMs(200);
            getCurVoltage();
        }
    }

    if(this->meter.curVoltage.HighValue != number.HighValue)
    {
        sendCommand("KN",false);
        sleepMs(100);


        while(this->meter.curVoltage.HighValue!=number.HighValue)
        {
            if(this->meter.curVoltage.HighValue < number.HighValue)
            {
                sendCommand("SV+",false);
            }
            else
            {
                sendCommand("SV-",false);
            }

            sleepMs(200);
            getCurVoltage();
        }

    }

}

void clsHVRs232::trig(bool value)
{
    if(value)
        sendCommand("KO",false);
    else
        sendCommand("KOD",false);
}

double clsHVRs232::getCurVoltage()
{
    QString  strRes = sendCommand("V",true);

    if(!strRes.isEmpty())
    {
        this->meter.curVoltage.setValue(strRes.mid(1,5));
        return meter.curVoltage.toValue();
    }

    return 0;
}

void clsHVRs232::setMaxPower(int value)
{
    if(!init())
        return;

    while((int)(meter.maxPower.toValue()) != value)
    {
        if(meter.maxPower.toValue() > value)
        {
            sendCommand("SP-",false);
        }
        else
        {
            sendCommand("SP+",false);
        }
        qApp->processEvents();
        sleepMs(200);
        getMaxPower();
    }

}

void clsHVRs232::setMaxVoltage(int value)
{
    if(!init())
        return;

    while((int)(meter.maxVoltage.toValue()) != value)
    {
        if(meter.maxVoltage.toValue() > value)
        {
            sendCommand("SU-",false);
        }
        else
        {
            sendCommand("SU+",false);
        }
        qApp->processEvents();
        sleepMs(100);
        getMaxVoltage();
    }
}

int clsHVRs232::getMaxPower()
{
    QString  strRes = sendCommand("P",true);

    if(!strRes.isEmpty())
    {
        this->meter.maxPower.HighValue = strRes.mid(1,3).toInt();
        this->meter.maxPower.PointValue=0;
        return meter.maxPower.toValue();
    }

    return 0;
}


int clsHVRs232::getMaxVoltage()
{
    QString  strRes = sendCommand("U",true);

    if(!strRes.isEmpty())
    {
        this->meter.maxVoltage.HighValue = strRes.mid(1,2).toInt();
        this->meter.maxVoltage.PointValue=0;
        return meter.maxVoltage.toValue();
    }

    return 0;
}

VoltageMeter clsHVRs232::getMeterStatus()
{

    QString  strValue = sendCommand("L",true);
    // V40.00A0.000W000.0U40I0.10P004F000010
    this->meter.curVoltage.setValue(strValue.mid(1,5));
    this->meter.curCurrent.setValue(strValue.mid(7,5));
    this->meter.curPower.setValue(strValue.mid(13,5));
    this->meter.maxVoltage.setValue(strValue.mid(19,2));
    this->meter.maxCurrent.setValue(strValue.mid(22,4));
    this->meter.maxPower.setValue(strValue.mid(27,3));

    this->meter.isOutPut =strValue.mid(31,1).toInt();
    return this->meter;
}

void clsHVRs232::disConnect()
{

}

void clsHVRs232::readData(QByteArray res)
{
    strRecive += QString(res);
    if(strRecive.contains('\n'))
        isFilled=true;
}

void clsHVRs232::timeOut()
{
    isTimeOut=true;
}

void clsHVRs232::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
