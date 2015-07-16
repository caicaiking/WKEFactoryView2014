#include "clsDataProcess.h"

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



