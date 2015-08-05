#ifndef WK3260_H
#define WK3260_H
#include <QString>
#include "clsRuningSettings.h"
#include "doubleType.h"
#define SP =" "

struct ItemType
{
    QString item1;
    QString item2;

    QString toText(int index)
    {
        if(index ==1)
            return item1;
        else
            return item2;
    }

    void setItem(QString item1, QString item2)
    {
        this->item1 = item1;
        if(item1=="Z")
        {
           this->item2="θ";
        }
        else
            this->item2 = item2;
    }

    QString toGpib(QString Meter=":MEAS")
    {
        QString tmp="";

        if(item1!="Z")
            tmp = tmp.append("%1:FUNC:%2;FUNC:%3")
                    .arg(Meter).arg(this->item1).arg(this->item2);
        else
            tmp = tmp.append("%1:FUNC:%2")
                    .arg(Meter).arg(this->item1);
        return tmp;

    }
};

struct LevelType
{
    double value;
    QString unit;

    QString toText()
    {
        doubleType dt;
        dt.setData(value, "");
        return dt.formateToString(5)+ unit;
    }

    void setLevel(double value, QString unit)
    {
        this->value =value;
        this->unit = unit;
    }

    QString toGpib(QString Meter=":MEAS")
    {
        QString tmp="";
        tmp = tmp.append("%1:LEV %2%3")
                .arg(Meter).arg(this->value).arg(this->unit);
        return tmp;
    }

};

struct FrequencyType
{
    double value;

    void setValue(double value)
    {
        this->value=value;
    }

    QString toText()
    {
        doubleType dt;
        dt.setData(value,"");

        return dt.formateToString(5)+"Hz";
    }

    QString toGpib(QString Meter=":MEAS")
    {
        QString tmp="";
        tmp = tmp.append("%1:FREQ %2")
                .arg(Meter).arg(this->value);
        return tmp;
    }
};

struct SpeedType
{
    QString value;

    void setValue(QString value)
    {
        this->value = value;
    }

    QString toText()
    {
        return this->value;
    }

    QString toGpib(QString Meter=":MEAS")
    {

        QString speed="";

        if(value ==QObject::tr("最快"))
            speed="MAX";
        else if(value ==QObject::tr("快速"))
            speed ="FAST";
        else if(value ==QObject::tr("中速"))
            speed ="MED";
        else if(value == QObject::tr("慢速"))
            speed ="SLOW";
        else
            speed="MAX";

        QString tmp="";
        tmp = tmp.append("%1:SPEED %2")
                .arg(Meter).arg(speed);
        return tmp;
    }
};

struct EqucctType
{
    QString value;

    void setValue(QString value)
    {
        this->value = value;
    }

    QString toText()
    {
        return this->value;
    }

    QString toGpib(QString Meter=":MEAS")
    {

        QString equcct="";

        if(value ==QObject::tr("串联"))
            equcct="SER";
        else if(value ==QObject::tr("并联"))
            equcct ="PAR";

        else
            equcct="SER";

        QString tmp="";
        tmp = tmp.append("%1:EQU-CCT %2")
                .arg(Meter).arg(equcct);
        return tmp;
    }


};

struct BiasValue
{
    double value;
    QString status;
    void setValue(double value,QString status)
    {
        this->value =value;
        this->status= status;
    }

    QString toText(int i)
    {
        if(i==1)
        {
            doubleType dt;
            dt.setData(this->value,"");
            return dt.formateToString(6)+"A";
        }
        else
        {
            return this->status;
        }
    }

    QString toGpib(QString Meter=":MEAS")
    {

        QString tmp="";
        tmp = tmp.append("%1:BIAS %2;BIAS %3")
                .arg(Meter).arg(QString::number(this->value)).arg(this->status);
        return tmp;
    }
};

struct BiasType
{
    QString value;

    void setValue(QString value)
    {
        this->value =value;
    }

    QString toText()
    {
        return this->value;
    }

    QString toGpib(QString Meter=":MEAS")
    {
        QString bias="";

        if(value ==QObject::tr("Norm"))
            bias="INT";
        else if(value ==QObject::tr("Boost"))
            bias ="EEXT";

        else
            bias="INT";

        QString tmp="";
        tmp = tmp.append("%1:BIAS %2")
                .arg(Meter).arg(bias);
        return tmp;
    }
};

struct BiasSpeedType
{
    QString value;

    void setValue(QString value)
    {
        this->value =value;
    }

    QString toText()
    {
        return this->value;
    }

    QString toGpib(QString Meter=":MEAS")
    {
        QString tmp="";
        tmp = tmp.append("%1:I-SWEEP %2")
                .arg(Meter).arg(value.toUpper());
        return tmp;
    }
};


struct WK3260
{
    ItemType item;
    LevelType level;
    FrequencyType freq;
    SpeedType speed;
    EqucctType equcct;
    BiasSpeedType biasSpeed;
    BiasType biasType;
    BiasValue biasValue;
    QString Meter;
};

#endif // WK3260_H
