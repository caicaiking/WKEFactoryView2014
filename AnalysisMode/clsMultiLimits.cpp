#include "clsMultiLimits.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
#include <QObject>
clsMultiLimits::clsMultiLimits()
{
    this->status = true;

    blLimit1 = false;
    blLimit2 = false;
}

clsMultiLimits::~clsMultiLimits()
{

}

void clsMultiLimits::setJsonString(QString json)
{
    QJsonParseError error;
    QJsonDocument jsDocument = QJsonDocument::fromJson(json.toUtf8(),&error);

    if(error.error == QJsonParseError::NoError)
    {
        if(jsDocument.isObject())
        {
            QVariantMap result = jsDocument.toVariant().toMap();
            blLimit1 = result["blLimit1"].toBool();
            blLimit2 = result["blLimit2"].toBool();

            QString tmp;
            tmp = result["Limit1"].toString();
            limit1.setString(tmp);

            tmp = result["Limit2"].toString();
            limit2.setString(tmp);

            tmp = result["Freq"].toString();
            freq.setString(tmp);
        }
    }



}

QString clsMultiLimits::toJsonString()
{
    QVariantMap mtLimits;
    mtLimits.insert("blLimit1",this->blLimit1);
     mtLimits.insert("blLimit2",this->blLimit2);

     QString tmp;
     tmp = limit1.toString();
     mtLimits.insert("Limit1",tmp);
     tmp = limit2.toString();
     mtLimits.insert("Limit2",tmp);

     tmp = freq.toString();
     mtLimits.insert("Freq",tmp);

     QJsonDocument jsDocument = QJsonDocument::fromVariant(mtLimits);
     if(!jsDocument.isNull())
         return jsDocument.toJson(QJsonDocument::Compact);
     else
         return "";
}

void clsMultiLimits::setItems(const QString &item1, const QString &item2)
{
    this->item1= item1;
    this->item2 = item2;
}

QString clsMultiLimits::getLimit1Show()
{
    if(blLimit1)
        return limit1.showLimits(UserfulFunctions::getSuffix(this->item1));
    else
        return QObject::tr("没有设定");
}

QString clsMultiLimits::getLimit2Show()
{
    if(blLimit2)
        return limit2.showLimits(UserfulFunctions::getSuffix(this->item2));
    else
        return QObject::tr("没有设定");
}

QString clsMultiLimits::getFreqRange()
{
    return freq.showLimits("Hz");
}

void clsMultiLimits::resetStatus()
{
    this->status=true;
}

void clsMultiLimits::compareValue( double &freq, double item1, double item2)
{
    if(!this->freq.comparaValue(freq,this->freq.getLimitType()))
        return;

    bool blItem1=true;
    bool blItem2=true;

    if(this->blLimit1)
        blItem1= limit1.comparaValue(item1,limit1.getLimitType());

    if(this->blLimit2)
        blItem2 = limit2.comparaValue(item2,limit2.getLimitType());

    status = status && blItem1 && blItem2;

}
clsMeterLimit clsMultiLimits::getFreq() const
{
    return freq;
}

void clsMultiLimits::setFreq(const clsMeterLimit &value)
{
    freq = value;
}
clsMeterLimit clsMultiLimits::getLimit1() const
{
    return limit1;
}

void clsMultiLimits::setLimit1(const clsMeterLimit &value)
{
    limit1 = value;
}
clsMeterLimit clsMultiLimits::getLimit2() const
{
    return limit2;
}

void clsMultiLimits::setLimit2(const clsMeterLimit &value)
{
    limit2 = value;
}
bool clsMultiLimits::getBlLimit2() const
{
    return blLimit2;
}

void clsMultiLimits::setBlLimit2(bool value)
{
    blLimit2 = value;
}
bool clsMultiLimits::getBlLimit1() const
{
    return blLimit1;
}

void clsMultiLimits::setBlLimit1(bool value)
{
    blLimit1 = value;
}






