#include "clsMultiCurveLimt.h"
#include <QDebug>
#include "doubleType.h"
clsMultiCurveLimt::clsMultiCurveLimt()
{

}

clsMultiCurveLimt::~clsMultiCurveLimt()
{

}

QStringList clsMultiCurveLimt::getWriteFileString()
{
    QStringList tmpList;
    for(int i=0; i<limits.length();i++)
    {
        double hiFreq = limits[i].getFreq().getAbsLimitHigh();
        double loFreq = limits[i].getFreq().getAbsLimitLow();
        bool status = limits[i].getStatus();
        QString strStatus = status?QObject::tr("通过"):QObject::tr("失败");
        doubleType dt;
        dt.setData(hiFreq);
        QString strHiFreq = dt.formateToString(6);
        dt.setData(loFreq);
        QString strLoFreq = dt.formateToString(6);
        QString tmpString=QString("%1Hz~%2Hz,%3").arg(strLoFreq).arg(strHiFreq).arg(strStatus);
        tmpList<<tmpString;
    }
    return tmpList;
}

int clsMultiCurveLimt::inLimit(const double &freq)
{
       for(int i=0; i< limits.length();i++)
       {
         bool value=  limits[i].getFreq().comparaValue(freq);
         if(value)
             return i;
       }

       return limits.length();
}

void clsMultiCurveLimt::compareValue(double freq, double item1, double item2)
{
    for(int i=0; i<this->limits.length();i++)
    {
        limits[i].compareValue(freq,item1,item2);
    }
}

void clsMultiCurveLimt::resetStatus()
{
    for(int i=0; i<this->limits.length();i++)
    {
        limits[i].resetStatus();
    }
}

bool clsMultiCurveLimt::getStatus()
{
    bool status=true;
    for(int i=0; i< this->limits.length();i++)
    {
        status = status && limits[i].getStatus();
        if(status==false)
            break;
    }
    return status;
}

bool clsMultiCurveLimt::hasEnableLimits()
{
    bool status=false;

    for(int i=0; i< this->limits.length();i++)
    {
        status = status || limits[i].hasEnaleLimit();
    }

    return status;
}

void clsMultiCurveLimt::readSettings()
{
    limits.clear();
    clsSettings settings;

    QString strNode="MultiCurveLimit/";
    QString tmpStr;
    settings.readSetting(strNode+"Limits",tmpStr);

    QJsonParseError error;
    QJsonDocument jsDocument = QJsonDocument::fromJson(tmpStr.toUtf8(),&error);
    if(error.error == QJsonParseError::NoError)
    {

        if(jsDocument.isArray())
        {
            QVariantList result = jsDocument.toVariant().toList();
            foreach (QVariant tmp, result)
            {
                QString tmpString = tmp.toString();
                clsMultiLimits limit;
                limit.setJsonString(tmpString);
                limits.append(limit);
            }
        }
    }


}

void clsMultiCurveLimt::setItem(QString item1, QString item2)
{
    for(int i=0;i< limits.length();i++)
    {
        limits[i].setItems(item1,item2);
    }
}

void clsMultiCurveLimt::writeSettings()
{
    clsSettings settings;

    QString strNode="MultiCurveLimit/";
    QList<QVariant> tp;
    for(int i=0; i< this->limits.length();i++)
    {
        clsMultiLimits tmp =limits.at(i);
        tp.append(tmp.toJsonString());
    }

    QJsonDocument jsDocument = QJsonDocument::fromVariant(tp);
    if(!jsDocument.isNull())
    {
        QString tmpJson = jsDocument.toJson(/*QJsonDocument::Compact*/);
       // qDebug()<< tmpJson;
        settings.writeSetting(strNode+"Limits",tmpJson);
    }
}

