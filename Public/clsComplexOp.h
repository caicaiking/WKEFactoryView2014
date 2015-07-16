#ifndef CLSCOMPLEXOP_H
#define CLSCOMPLEXOP_H


struct CPLXTrig
{
    double AbsValue;
    double Angle;
};

struct CPLXAlge
{
    double Real;
    double Imag;
};

enum Equcct
{
    series=0,parallel=1
};

const double PI=3.1415926;

class clsComplexOp
{
public:

    clsComplexOp(double inZ, double inA, double inFreq,
                 Equcct inEqucct);
    void CalaculateParameters();

    //三角形式复数除法
    CPLXTrig CPLXDiviT(CPLXTrig Dividend, CPLXTrig Divisor);
    double Abs(const double &value);
    CPLXAlge TrigToAlge(CPLXTrig Trig);
    double QCal();

    double C(){return mC;}
    double D(){return mD;}
    double L(){return mL;}
    double Q(){return mQ;}
    double G(){return mG;}
    double B(){return mB;}
    double R(){return mR;}
    double X(){return mX;}
    double Z(){return mZ;}
    double A(){return mA;}
    double Y(){return mYY;}
private:
    double mC ;
    double mL;
    double mX;
    double mB;
    double mZ;
    double mYY;
    double mQ;
    double mD;
    double mR;
    double mG;
    double mA;
    Equcct mEqucct ;
    double mFreq;
    double mW;


};

#endif // CLSCOMPLEXOP_H
