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
    openData.AbsValue = z;
    openData.Angle =angle;
}

void clsDataProcess::applyShortData(double z, double angle)
{
    shortData.AbsValue =z;
    shortData.Angle = angle;
}


void clsDataProcess::doCalibration()
{
    //先进行短路的扣除

    clsComplexOp testData (this->mZ, this->mA, this->mFreq, series);
    clsComplexOp shortData(this->shortData.AbsValue,this->shortData.Angle,this->mFreq,series);
    clsComplexOp openData(this->openData.AbsValue,this->openData.Angle,this->mFreq,series);

    double calL, calR;
    calL = testData.L()- shortData.L();
    calR = testData.R() - shortData.R();
    if(calR == 0.0)
        calR = 1E-6;

    double calX = 2.0*PI*mFreq*calL;

    double calZ = sqrt(calR*calR + calX*calX);
    double calA = atan(calX/calR)*180.0/PI;

    qDebug()<< "calA " << calA;

    clsComplexOp calData(calZ,calA,mFreq,series);

    //在进行开路的扣除
    double calC= calData.C()-openData.C();
    calR = calData.R();

    calX = -1.0/(2*PI*mFreq*calC);

    calZ = sqrt(calX*calX + calR*calR);
    calA = atan(calX/calR)*180.0/PI;

    this->mZ = calZ;
    this->mA = calA;
    qDebug()<< "calA1 " << calA;
}

double clsDataProcess::getItem(QString item, QString equcct)
{
    double angle = (equcct==QObject::tr("串联") ? 1.0 : -1.0) *this->mA;
    Equcct equ =  (equcct==QObject::tr("串联") ? series :parallel);

    qDebug()<< angle;

    qDebug()<< equ;

    clsComplexOp data(this->mZ,angle,this->mFreq,equ);

    if(item =="R")
        return data.R();
    else if(item=="X")
        return data.X();
    else if(item =="Y")
        return data.Y();
    else if(item == "Z")
        return data.Z();
    else if(item == "A")
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
        return 99.999E9;

}



