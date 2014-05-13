#include "clsMeterLimit.h"
#include <QStringList>
clsMeterLimit::clsMeterLimit()
{
    limitType="Norm";
    absHi=0;
    absLo=0;
    norminal=0;
    perHi=0;
    perLo=0;
}

double clsMeterLimit::getAbsLimitHigh()
{
    if(limitType=="Norm")
    {
        return absHi;
    }
    else
    {
        return (1.0+perHi/100.0)*norminal;
    }
}

bool clsMeterLimit::operator ==(clsMeterLimit value)
{
    if(this->limitType != value.getLimitType())
        return false;

    if(this->limitType =="Norm")
    {
        if( this->absHi != value.getAbsHi() ||
                this->absLo != value.getAbsLo())
            return false;
    }
    else {
        if(this->norminal != value.getNorminal() ||
                this->perHi != value.getPerHi() ||
                this->perLo != value.getPerLo())
            return false;

    }
    return true;
}

double clsMeterLimit::getAbsLimitLow()
{
    if(limitType=="Norm")
    {
        return absLo;
    }
    else
    {
        return (1+perLo/100)*norminal;
    }
}

QString clsMeterLimit::toString()
{
    return QString("%1,%2,%3,%4,%5,%6")
            .arg(this->limitType)
            .arg(this->absHi)
            .arg(this->absLo)
            .arg(this->norminal)
            .arg(this->perHi)
            .arg(this->perLo);
}

//"Per,100,10,100,5,-5"
void clsMeterLimit::setString(QString value)
{
    QStringList values = value.split(",");
    if(value.length()<6)
        return;

    limitType = values.at(0);
    absHi = values.at(1).toDouble();
    absLo = values.at(2).toDouble();
    norminal = values.at(3).toDouble();
    perHi = values.at(4).toDouble();
    perLo = values.at(5).toDouble();

}
double clsMeterLimit::getNorminal() const
{
    return norminal;
}

void clsMeterLimit::setNorminal(double value)
{
    norminal = value;
}
double clsMeterLimit::getPerHi() const
{
    return perHi;
}

void clsMeterLimit::setPerHi(double value)
{
    perHi = value;
}
double clsMeterLimit::getPerLo() const
{
    return perLo;
}

void clsMeterLimit::setPerLo(double value)
{
    perLo = value;
}
double clsMeterLimit::getAbsHi() const
{
    return absHi;
}

void clsMeterLimit::setAbsHi(double value)
{
    absHi = value;
}
double clsMeterLimit::getAbsLo() const
{
    return absLo;
}

void clsMeterLimit::setAbsLo(double value)
{
    absLo = value;
}

QString clsMeterLimit::getLimitType() const
{
    return limitType;
}

void clsMeterLimit::setLimitType(const QString &value)
{
    limitType = value;
}






