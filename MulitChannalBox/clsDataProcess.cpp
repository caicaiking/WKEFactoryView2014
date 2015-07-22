#include "clsDataProcess.h"
#include <math.h>
#include <QObject>
#include <QDebug>

clsDataProcess::clsDataProcess(double z, double a, double freq)
{
    setFrequency(freq);
    setAngle(a);
    setImpedance(z);
}


void clsDataProcess::setFrequency(double value)
{
    this->mFreq = value;
}

void clsDataProcess::setImpedance(double value)
{
    this->mZ = value;
}

void clsDataProcess::setAngle(double value)
{
    this->mA = value;
}

void clsDataProcess::applyOpenData(double z, double angle)
{
    Zo.setZA(z,angle);
}

void clsDataProcess::applyShortData(double z, double angle)
{
    Zs.setZA(z, angle);
}

void clsDataProcess::applyLoadData(double z, double angle)
{
    Zsm.setZA(z,angle);
}

void clsDataProcess::applyStdData(double z, double anlge)
{
    Zstd.setZA(z,anlge);
}

void clsDataProcess::useLoadData(bool value)
{
    this->blUseLoad = value;
}


void clsDataProcess::doCalibration()
{
    complexType Zxm;     //测试值
    complexType Zdut;

    Zxm.setZA(mZ,mA);

    if(blUseLoad)
    {
        Zdut = Zstd *(Zs-Zxm)*(Zsm-Zo)/(Zxm-Zo)/(Zs-Zsm);
    }
    else
    {
        complexType one;
        one.setRX(1.0,0.0);

        complexType Yo;
        Yo.setZA(1.0/Zo.getZ(),-1.0*Zo.getA());

        Zdut =(Zxm - Zs)/(one-Yo*(Zxm-Zs));
    }

    mZ = Zdut.getZ();
    mA = Zdut.getA();
}

double clsDataProcess::getItem(QString item, QString equcct)
{
    double angle = (equcct==QObject::tr("串联") ? 1.0 : -1.0) *this->mA;
    Equcct equ =  (equcct==QObject::tr("串联") ? series :parallel);

//    qDebug()<< angle;

//    qDebug()<< equ;

    clsComplexOp data(this->mZ,angle,this->mFreq,equ);

    if(item =="R")
        return data.R();
    else if(item=="X")
        return data.X();
    else if(item =="Y")
        return data.Y();
    else if(item == "Z")
        return data.Z();
    else if((item == "A") || (item ==QString("θ")))
        return data.A();
    else if(item == "C")
        return data.C();
    else if(item == "D")
        return data.D();
    else if(item =="G")
        return data.G();
    else if(item == "L")
        return data.L();
    else if(item == "Q")
        return data.Q();
    else if(item =="B")
        return data.B();
    else
        return 99.999E12;

}



