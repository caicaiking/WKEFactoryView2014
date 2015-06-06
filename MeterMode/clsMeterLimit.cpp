#include "clsMeterLimit.h"
#include <QStringList>
#include "doubleType.h"

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
//"Norm,100,10,100,5,-5"
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
/*!
 * \brief clsMeterLimit::comparaValue
 * \param value
 * \param type 失败的类型
 * \return
 */
bool clsMeterLimit::comparaValue(double value,QString &type)
{
    type="PASS";
    if(limitType=="Norm")
    {
        if(absHi==0.0 && absLo==0.0)
            return true;

        if(absLo>value)
        {
            type="LOW";
            return false;
        }
        if(absHi< value)
        {
            type="HIGH";
            return false;
        }

        return true;
    }
    else
    {
        double hi,lo;
        hi = this->norminal*(1+perHi/100.0);
        lo = this->norminal*(1+perLo/100.0);

        if(hi==0.0 && lo==0.0)
            return true;

        if(hi< value)
        {
            type="HIGH";
            return false;
        }

        if(lo>value)
        {
            type="LOW";
            return false;
        }
        return true;
    }
}

bool clsMeterLimit::comparaValue(double value)
{
    QString tmpString;
    return  comparaValue(value,tmpString);
}

QString clsMeterLimit::showLimits(QString suffix)
{
    QString tmp;
    doubleType dt;

    if(this->limitType=="Norm")
    {
        dt.setData(this->absHi,"");
        tmp.append(QObject::tr("上限："));
        tmp.append(dt.formateToString(6));
        tmp.append(suffix);
        tmp.append("\n");
        tmp.append(QObject::tr("下限："));
        dt.setData(this->absLo,"");
        tmp.append(dt.formateToString(6));
        tmp.append(suffix);
    }
    else
    {
        dt.setData(this->norminal,"");
        QString nor=dt.formateToString(6);
        dt.setData(this->perHi,"");
        QString pH = dt.formateWithUnit("",5)+"%";
        dt.setData(this->perLo,"");
        QString lH = dt.formateWithUnit("",5)+"%";
        QString tmp2=QObject::tr(
                    "<table border=\"0\"  valign=\"middle\">"
                    "<tr>"
                    "<th rowspan=\"2\">%1%2</th>"
                    "<td>%3</td>"
                    "</tr>"
                    "<tr>"
                    "<td>%4</td>"
                    "</tr>"
                    "</table>")
                .arg(nor).arg(suffix).arg(pH).arg(lH);

        return tmp2;
    }

    return tmp;
}




