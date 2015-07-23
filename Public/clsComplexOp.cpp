#include "clsComplexOp.h"
#include <stdlib.h>
#include <math.h>

using namespace std;

clsComplexOp::clsComplexOp(double inZ, double inA, double inFreq, Equcct inEqucct)
{
    this->mZ=inZ;
    this->mA=inA;
    if(this->mA==0)
        this->mA= 1E-6;
    this->mFreq=inFreq;
    this->mEqucct=inEqucct;
    mW=2.0*PI*mFreq;
    CalaculateParameters();
}

void clsComplexOp::CalaculateParameters()
{
    CPLXTrig ZA;
    CPLXTrig YX;
    CPLXAlge RX;
    switch(mEqucct)
    {
    case series:


        ZA.AbsValue=mZ;
        ZA.Angle=mA;


        YX.AbsValue=1.0;
        YX.Angle=0.0;

        YX=CPLXDiviT(YX,ZA);
        mYY=YX.AbsValue;


        RX=TrigToAlge(ZA);
        mR=RX.Real;
        mX=RX.Imag;

        mL=mX/mW;
        mC=-1.0/(mX*mW);

        mB=1/mX;

        if(mX>0)
        {
            mQ=2.0*PI*mL*mFreq/mR;
            mD=1/mQ;
        }
        else
        {
            mD=mW*mC*mR;
            mQ=1/mD;
        }

        mG=sqrtf(Abs(mYY * mYY * mB * mB / (mB * mB - mYY * mYY)));

        break;
    case parallel:
        mQ=QCal();
        mD=1.0/mQ;

        ZA.AbsValue=mZ;
        ZA.Angle=mA;
        ZA.Angle=-mA;

        YX.AbsValue=1.0;
        YX.Angle=0.0;

        YX=CPLXDiviT(YX,ZA);
        mYY=YX.AbsValue;

        CPLXAlge GB=TrigToAlge(YX);
        mG=GB.Real;
        mB=GB.Imag;

        mC=mB/mW;
        mL=-1.0/(mW*mB);

        mR=Abs(mQ/(mW*mC));
        if(mA>0.0)
        {
            mX=sqrtl(mZ * mZ * mR * mR) / sqrtl(mR * mR - mZ * mZ);
        }
        else
        {
            mX=-1.0*sqrtl(mZ * mZ * mR * mR) / sqrtl(mR * mR - mZ * mZ);
        }


        break;
    }
}

double clsComplexOp::QCal()
{
    CPLXTrig ZA;
    ZA.AbsValue=mZ;
    ZA.Angle=-mA;

    CPLXAlge RX;
    RX=TrigToAlge(ZA);
    mR=RX.Real;
    mX=RX.Imag;

    mL=mX/mW;
    mC=-1.0/(mX*mW);

    if(mX>0)
    {
        mQ = 2.0 * PI * mL * mFreq / mR;
        mD = 1.0 / mQ;
    }
    else
    {
        mD = mW * mC * mR;
        mQ = 1.0 / mD;
    }
    return mQ;
}


CPLXAlge clsComplexOp::TrigToAlge(CPLXTrig Trig){
    //三角形式到代数形式的换算
    CPLXAlge xx;
    double real = Trig.AbsValue * cos(Trig.Angle *PI/180.0);
    double image = Trig.AbsValue * sin(Trig.Angle *PI/180.0);

    xx.Real=real;
    xx.Imag=image;
    if(Abs(xx.Imag)<0.000000000000001)
    {
        xx.Imag=0;
    }
    return xx;
}

double clsComplexOp::Abs(const double &value)
{
    if(value >0)
        return value;
    else
        return -1.0*value;
}
CPLXTrig clsComplexOp::CPLXDiviT(CPLXTrig Dividend  , CPLXTrig Divisor)
{
    //三角形式复数除法
    CPLXTrig CPLXDiviT;
    CPLXDiviT.AbsValue = Dividend.AbsValue / Divisor.AbsValue;
    CPLXDiviT.Angle = Dividend.Angle - Divisor.Angle;
    return CPLXDiviT;
}
