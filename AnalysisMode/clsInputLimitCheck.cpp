#include "clsInputLimitCheck.h"

timeLimitCheck::timeLimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue=0;
    maxValue=999999;
}

double timeLimitCheck::compareMin(double /*value*/)
{
    return minValue;
}

double timeLimitCheck::compareMax(double value)
{
    if(value > maxValue)
        return maxValue;
    else
        return value;
}

void timeLimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}

frequencyLimitCheck::frequencyLimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue =meter->getMinFrequency();
    maxValue =meter->getMaxFrequency();
}

double frequencyLimitCheck::compareMin(double value)
{
    if(this->minValue > value)
        return minValue;
    else
        return value;
}

double frequencyLimitCheck::compareMax(double value)
{
    if(this->maxValue < value)
        return maxValue;
    else
        return compareMin(value);
}

void frequencyLimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter = meter;
}


biasVLimitCheck::biasVLimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue =meter->getMinBiasV();
    maxValue =meter->getMaxBiasV();
}

double biasVLimitCheck::compareMin(double value)
{
    if(this->minValue > value)
        return minValue;
    else
        return value;
}

double biasVLimitCheck::compareMax(double value)
{
    if(this->maxValue < value)
        return maxValue;
    else
        return compareMin(value);
}

void biasVLimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}

biasALimitCheck::biasALimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue =meter->getMinBiasA();
    maxValue =meter->getMaxBiasA();
}

double biasALimitCheck::compareMin(double value)
{
    if(this->minValue > value)
        return minValue;
    else
        return value;
}

double biasALimitCheck::compareMax(double value)
{
    if(this->maxValue < value)
        return maxValue;
    else
        return compareMin(value);
}

void biasALimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}


levelVLimitCheck::levelVLimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue =meter->getMinLevelV();
    maxValue =meter->getMaxLevelV();
}

double levelVLimitCheck::compareMin(double value)
{
    if(this->minValue > value)
        return minValue;
    else
        return value;
}

double levelVLimitCheck::compareMax(double value)
{
    if(this->maxValue < value)
        return maxValue;
    else
        return compareMin(value);
}

void levelVLimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}


levelALimitCheck::levelALimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue =meter->getMinLevelA();
    maxValue =meter->getMaxLevelA();
}

double levelALimitCheck::compareMin(double value)
{
    if(this->minValue > value)
        return minValue;
    else
        return value;
}

double levelALimitCheck::compareMax(double value)
{
    if(this->maxValue < value)
        return maxValue;
    else
        return compareMin(value);
}

void levelALimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}



LimitCheck *LimitCheckFactory::getLimitCheckType(SweepType t, WKEInstrument *meter)
{
    switch (t) {
    case Time:
        return new timeLimitCheck(meter);
        break;
    case Frequency:
        return new frequencyLimitCheck(meter);
        break;
    case BiasV:
        return new biasVLimitCheck(meter);
        break;
    case BiasExtV:
        return new biasExtLimitCheck(meter);
        break;
    case BiasA:
        return new biasALimitCheck(meter);
        break;
    case levelV:
        return new levelVLimitCheck(meter);
        break;
    case levelA:
        return new levelALimitCheck(meter);
        break;
    default:
        return (LimitCheck *)0;
        break;
    }
}


LimitCheck::LimitCheck(WKEInstrument *meter)
{
   this->meter = meter;
}

biasExtLimitCheck::biasExtLimitCheck(WKEInstrument *meter):LimitCheck(meter)
{
    minValue=0;
    maxValue=200;
}

double biasExtLimitCheck::compareMin(double value)
{
    if(value < minValue)
        return minValue;
    else
        return value;
    return minValue;
}

double biasExtLimitCheck::compareMax(double value)
{
    if(value > maxValue)
        return maxValue;
    else
        return value;
}

void biasExtLimitCheck::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}
