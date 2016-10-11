#include "cls6500ResonaceMode.h"
#include "clsRuningSettings.h"
#include <QDebug>

void cls6500ResonaceMode::setStart(double value)
{
    QString gpibCmd=QString("%1ST %2").arg(RESONANCE).arg(QString::number(value));
    clsRS::getInst().sendCommand(gpibCmd,false);
}

void cls6500ResonaceMode::setStop(double value)
{
    QString gpibCmd=QString("%1SP %2").arg(RESONANCE).arg(QString::number(value));
    clsRS::getInst().sendCommand(gpibCmd,false);
}

void cls6500ResonaceMode::setSpeed(QString value)
{
    QString myValue;

    if(value ==QObject::tr("最快"))
    {
        myValue="MAX";
    }
    else if(value ==QObject::tr("快速"))
    {
        myValue="FAST";
    }
    else if(value ==QObject::tr("中速"))
    {
        myValue="MED";
    }
    else if(value ==QObject::tr("慢速"))
    {
        myValue="SLOW";
    }
    else
    {
        myValue="MAX";
    }

    QString gpibCmd=QString("%1SPEED %2").arg(RESONANCE).arg(myValue);
    clsRS::getInst().sendCommand(gpibCmd,false);
}

void cls6500ResonaceMode::setEqucct(QString value)
{
    QString myValue;

    if(value == QObject::tr("串联"))
    {
        myValue="SER";
    }
    else if(value == QObject::tr("并联"))
    {
        myValue="PAR";
    }
    else if(value == QObject::tr("晶体谐振器"))
    {
        myValue="XTAL";
    }
    else
    {
        myValue="SER";
    }

    QString gpibCmd=QString("%1EQU-CCT %2").arg(RESONANCE).arg(myValue);
    //qDebug()<< gpibCmd;
    clsRS::getInst().sendCommand(gpibCmd,false);
}

QString cls6500ResonaceMode::trig()
{
    QString strRet = clsRS::getInst().sendCommand(":METER:RESO:TRIG",true,10);
    return strRet;
}

void cls6500ResonaceMode::setDepth(int value)
{
    QString gpibCmd=QString("%1DEPTH %2").arg(RESONANCE).arg(QString::number(value));
    clsRS::getInst().sendCommand(gpibCmd,false);
}

void cls6500ResonaceMode::setCheckCp(bool value)
{
    QString gpibCmd;
    if(value)
    {
        gpibCmd=QString("%1ENABLE-CP %2").arg(RESONANCE).arg(1);
    }
    else
    {
        gpibCmd=QString("%1ENABLE-CP %2").arg(RESONANCE).arg(0);
    }
    clsRS::getInst().sendCommand(gpibCmd,false);
}



double cls6500ResonaceMode::getStart()
{
    QString gpibCmd=QString("%1ST?").arg(RESONANCE);
    QString strRet = clsRS::getInst().sendCommand(gpibCmd,true);
    return strRet.toDouble();
}

double cls6500ResonaceMode::getStop()
{
    QString gpibCmd=QString("%1SP?").arg(RESONANCE);
    QString strRet = clsRS::getInst().sendCommand(gpibCmd,true);
    return strRet.toDouble();
}



QString cls6500ResonaceMode::getSpeed()
{
    QString gpibCmd=QString("%1SPEED?").arg(RESONANCE);
    QString strRet = clsRS::getInst().sendCommand(gpibCmd,true);
    //qDebug()<< "Equcct "<< strRet;
    int ret = strRet.toInt();
    switch (ret) {
    case -4:
        return QObject::tr("最快");
        break;
    case -3:
        return QObject::tr("快速");
        break;
    case -2:
        return QObject::tr("中速");
        break;
    case -1:
        return QObject::tr("慢速");
        break;

    default:
        break;
    }
    return QObject::tr("最快");
}

int cls6500ResonaceMode::getDepth()
{
    QString gpibCmd=QString("%1DEPTH?").arg(RESONANCE);
    QString strRet = clsRS::getInst().sendCommand(gpibCmd,true);
    return strRet.toInt();
}

bool cls6500ResonaceMode::getCheckCp()
{
    QString gpibCmd=QString("%1ENABLE-CP?").arg(RESONANCE);
    QString strRet = clsRS::getInst().sendCommand(gpibCmd,true);

    return (strRet =="1"?true:false);
}

QString cls6500ResonaceMode::training()
{
    QString strReturn =clsRS::getInst().sendCommand(":METER:RESO:TRAIN",true,10);
    return strReturn;
}

QString cls6500ResonaceMode::getEqucct()
{
    QString gpibCmd=QString("%1EQU-CCT?").arg(RESONANCE);
    QString strRet = clsRS::getInst().sendCommand(gpibCmd,true);
    qDebug()<< "Equcct "<< strRet;
    int ret = strRet.toInt();
    switch (ret) {
    case 0:
        return QObject::tr("串联");
        break;
    case 1:
        return QObject::tr("并联");
        break;
    case 2:
        return QObject::tr("晶体谐振器");
        break;
    default:
        break;
    }
    return "";
}

QString cls6500ResonaceMode::getFmFn()
{
    QString strReturn =clsRS::getInst().sendCommand(":METER:RESO:FM-FN?",true);
    return strReturn;
}

QString cls6500ResonaceMode::getFsFp()
{
    QString strReturn =clsRS::getInst().sendCommand(":METER:RESO:FS-FP?",true);
    return strReturn;
}

double cls6500ResonaceMode::getCp1K()
{
    QString strReturn =clsRS::getInst().sendCommand(":METER:RESO:CP-1K?",true);
    return strReturn.toDouble();
}

double cls6500ResonaceMode::getKeff()
{
    QString strReturn =clsRS::getInst().sendCommand(":METER:RESO:KEFF?",true);
    return strReturn.toDouble();
}
