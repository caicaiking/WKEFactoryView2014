#include "doubleType.h"
#include <math.h>



double doubleType::getData(const double orgValue, const QString orgUnit)
{

    if (unitMap.count()  <1 )
        initUnitMap();

    this->unit ="";

    if (orgValue==0)
    {
        this->value=0.0;
        return 0.0;
    }

    QString unit= orgUnit;
    unit= unit.remove(" ");

    int unitIndex = unitMap[unit];


    switch(unitIndex)
    {
    case (18):
        this->value=orgValue *1e18;
        return this->value;
       case (15):
        this->value= orgValue *1e15;
        return this->value;
    case (12):
        this->value=orgValue *1e12;
        return this->value;
    case (9):
        this->value= orgValue *1e9;
        return this->value;
           case (6):
        this->value=orgValue *1e6;
        return this->value;
    case (3):
        this->value= orgValue *1e3;
        return this->value;
    case (1):
        this->value=orgValue *1;
        return this->value;
    case (-3):
        this->value= orgValue *1e-3;
        return this->value;
    case (-6):
        this->value=orgValue *1e-6;
        return this->value;
    case (-9):
        this->value=orgValue *1e-9;
        return this->value;
    case (-12):
        this->value=orgValue *1e-12;
        return this->value;
    case (-15):
        this->value= orgValue *1e-15;
        return this->value;
    case (-18):
        this->value=orgValue *1e-18;
        return this->value;
    }

    unit = unit.toLower();
    if(unit.left(1)=="e")
    {
        QString strData = unit.mid(1);
        bool ok;
        strData.toDouble(&ok);
        if(ok)
        {
            QString newUnit = "1"+unit;
            this->value= orgValue *newUnit.toDouble();
            return this->value;
        }

    }

    return this->value;
}

double doubleType::Data()
{
    double exValue= this->value;
    QString exUnit= this->unit;
    return getData(exValue,exUnit);
}

//如 传入数据位 10000， Unit =k 则返回值位10
double doubleType::getDataWithUnit(QString unit)
{
    double data = Data();

    int Exx= unitMap.value(unit);

    return data/pow(10.0,Exx);
}

void doubleType::initUnitMap()
{
    unitMap.insert("E",18);
    unitMap.insert("P",15);
    unitMap.insert("T",12);
    unitMap.insert("G",9);
    unitMap.insert("M",6);
    unitMap.insert("k",3);
    unitMap.insert("K",3);
    unitMap.insert("",0);
    unitMap.insert(" ",0);
    unitMap.insert("m",-3);
    unitMap.insert("u",-6);
    unitMap.insert("n",-9);
    unitMap.insert("p",-12);
    unitMap.insert("f",-15);
    unitMap.insert("a",-18);
}

int doubleType :: Abs(double & value)
{
    if (value >=0)
        return 1;
    else
    {
        value *=-1.0;
        return -1;
    }
}

void  doubleType::setData(const double orgValue, const QString orgUnit)
{
    this->value= orgValue;
    this->unit= orgUnit;

    if (this->value ==0.0 )
    {

        this->unit ="";
        this->value=0.0;

        return;
    }


    double getValue = getData(this->value,this->unit);
    this->value= getValue;
    this->unit="";

    int factor = Abs(getValue);

    if (getValue / 1e18 >=1000)
    {
        int loop =20;
        getValue= getValue /pow(10.0,loop);
        while(getValue>=10.0000)
        {
            getValue/=10;
            loop+=1;
        }

        this->unit  = "e"+QString::number(loop);
        this->value= factor * getValue;
        return;
    }
    else if(getValue / 1e15 >=1000)
    {
        this->unit ="E";
        this->value=  factor *getValue/pow(10.00,18);
        return;
    }
    else if(getValue / 1e12 >=1000)
    {
         this->unit ="P";
        this->value= factor *getValue/pow(10.00,15);
        return;
    }
    else if(getValue / 1e9 >=1000)
    {
        this->unit ="T";
        this->value= factor *getValue/pow(10.00,12);
        return;
    }
    else if(getValue / 1e6 >=1000)
    {
        this->unit ="G";
        this->value= factor *getValue/pow(10.00,9);
        return;
    }
    else if(getValue / 1e3 >=1000)
    {
        this->unit ="M";
        this->value= factor *getValue/pow(10.00,6);
        return;

    }
    else if(getValue / 1e0 >=1000)
    {
        this->unit ="k";
        this->value=factor *getValue/pow(10.00,3);
        return;
    }
    else if(getValue * 1e3 >=1000)
    {
        this->unit ="";
        this->value= factor *getValue/pow(10.00,0);
        return;
    }
    else if(getValue * 1e6 >=1000)
    {
        this->unit ="m";
        this->value= factor *getValue*pow(10.00,3);
        return;
    }
    else if(getValue * 1e9 >=1000)
    {
        this->unit ="u";
        this->value= factor *getValue*pow(10.00,6);
        return;
    }
    else if(getValue * 1e12 >=1000)
    {
        this->unit ="n";
        this->value= factor *getValue*pow(10.00,9);
        return;
    }
    else if(getValue * 1e15 >=1000)
    {
        this->unit ="p";
        this->value= factor *getValue*pow(10.00,12);
        return;
    }
    else if(getValue * 1e18 >=1000)
    {
        this->unit ="f";
        this->value= factor *getValue*pow(10.00,15);
        return;
    }
    else if(getValue * 1e21 >=1000)
    {
        this->unit ="a";
        this->value= factor *getValue*pow(10.00,18);
        return;
    }
    else
    {
        int loop=-19;
        getValue=getValue/pow(10.00,loop);
        while(getValue>=10.00)
        {
            getValue=getValue *10.00;
            loop-=1;
        }
        this->unit ="e"+ QString::number(loop);
        this->value=factor * getValue;
        return;
    }

}



doubleType::doubleType(double exValue, QString exUnit)
{

    initUnitMap();
    setData(exValue,exUnit);

}

QString doubleType::formateToString(int length)
{

    QString exUnit=this->unit;
    double exValue=this->value;

    setData(exValue,exUnit);


    QString strForFormat=QString::number(this->value,'f',7);

    if(strForFormat.length ()>length)
        strForFormat = strForFormat.left (length);

    return strForFormat.left(length) + this->unit;
}

QString doubleType::formateWithUnit(QString unit, int length)
{
    double data = Data();

    int Exx= unitMap.value(unit);

    data=data/pow(10.0,Exx);

    QString strForFormat=QString::number(data,'f',7);

    if(strForFormat.length ()>length)
        strForFormat = strForFormat.left (length);

    return strForFormat.left(length);

}

void doubleType::getDataAndUnit(double &exValue, QString &exUnit)
{
    exValue= this->value;
    exUnit= this->unit;
}

