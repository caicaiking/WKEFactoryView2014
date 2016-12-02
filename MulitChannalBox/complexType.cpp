#include "complexType.h"
#include <cmath>
#include <QString>

void complexType::setRX(double r, double x)
{
    this->mR = r;
    this->mX = x;

    convertRxToZa();
}

void complexType::setZA(double z, double a)
{
    this->mZ= z;
    this->mA =a;

    convertZaToRx();
}

void complexType::convertRxToZa()
{
    mZ = sqrt(mR*mR + mX* mX);

    if(mR> 0)
    {
        mA = atan(mX/mR)*180.0/3.1415926;
    }
    else
    {
        if(mX> 0)
            mA = 180.0- atan(mX/-mR)*180.0/3.1415926;
        else
            mA = -180 - atan(mX/-mR)*180.0/3.1415926;
    }
}

void complexType::convertZaToRx()
{
    mR = mZ * cos(mA/180.0*3.1415926);
    mX = mZ * sin(mA/180.0*3.1415926);
}

double complexType::getR() const
{
    return mR;
}

void complexType::setR(double value)
{
    mR = value;
}
double complexType::getX() const
{
    return mX;
}

void complexType::setX(double value)
{
    mX = value;
}
double complexType::getZ() const
{
    return mZ;
}

void complexType::setZ(double z)
{
    mZ = z;
}
double complexType::getA() const
{
    return mA;
}

void complexType::setA(double a)
{
    mA = a;
}

complexType complexType::operator +(complexType value)
{
    std::complex<double> a(this->getR(),this->getX());
    std::complex<double> b(value.getR(),value.getX());

    std::complex<double> c = a+b;
    complexType tmp;
    tmp.setRX(c.real(),c.imag());
    return tmp;
}

complexType complexType::operator -(complexType value)
{
    std::complex<double> a(this->getR(),this->getX());
    std::complex<double> b(value.getR(),value.getX());

    std::complex<double> c = a-b;
    complexType tmp;
    tmp.setRX(c.real(),c.imag());
    return tmp;
}

complexType complexType::operator *(complexType value)
{
    std::complex<double> a(this->getR(),this->getX());
    std::complex<double> b(value.getR(),value.getX());

    std::complex<double> c = a*b;
    complexType tmp;
    tmp.setRX(c.real(),c.imag());
    return tmp;

}

complexType complexType::operator /(complexType value)
{
    std::complex<double> a(this->getR(),this->getX());

    std::complex<double> c1;

    if(value.getR()!=0)
        c1=std::complex<double>(value.getR(),value.getX());
    else
        c1=std::complex<double> (1E-19,value.getX());

    std::complex<double> c = a/c1;
    complexType tmp;
    tmp.setRX(c.real(),c.imag());
    return tmp;
}

QString complexType::toString()
{
    QString tmpStr;
    tmpStr = QString("R X:\t %3 + %4j").arg(mR).arg(mX);
    return tmpStr;
}
