#ifndef CLSCOMPLEXADVANCE_H
#define CLSCOMPLEXADVANCE_H

#include <complex>
#include <QString>
class complexType
{
public:
    complexType(){}


    void setRX(double r, double x);
    void setZA(double z, double a);

    void convertRxToZa();
    void convertZaToRx();

    double getR() const;
    void setR(double value);

    double getX() const;
    void setX(double value);

    double getZ() const;
    void setZ(double z);

    double getA() const;
    void setA(double a);

    complexType operator +(complexType value);
    complexType operator -(complexType value);
    complexType operator *(complexType value);
    complexType operator /(complexType value);
    QString toString();

private:
    double mR;
    double mX;

    double mZ;
    double mA;


};

#endif // CLSCOMPLEXADVANCE_H
