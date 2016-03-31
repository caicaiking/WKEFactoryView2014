#ifndef CLSINPUTLIMITCHECK_H
#define CLSINPUTLIMITCHECK_H
#include "Globle.h"
#include "WKEInstrument.h"

/*!
  用于仪器的参数范围检查
*/
class LimitCheck
{
public:
    LimitCheck(WKEInstrument *meter);
    virtual double compareMin(double value)=0;
    virtual double compareMax(double value)=0;
    virtual void setMeter(WKEInstrument *meter)=0;
protected:
    WKEInstrument *meter;
};


class timeLimitCheck:public LimitCheck
{
public:
    timeLimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
    void setMeter(WKEInstrument * meter);

private:
    double minValue ;
    double maxValue ;
  //  WKEInstrument *meter;
};

class biasExtLimitCheck:public LimitCheck
{
public:
    biasExtLimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
    void setMeter(WKEInstrument * meter);

private:
    double minValue ;
    double maxValue ;
  //  WKEInstrument *meter;
};



class frequencyLimitCheck:public LimitCheck
{
public:
    frequencyLimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
    void setMeter(WKEInstrument * meter);

private:
    double minValue ;
    double maxValue ;
    // WKEInstrument *meter;
};

class biasVLimitCheck:public LimitCheck
{
public:
    biasVLimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
 void setMeter(WKEInstrument * meter);
private:
    double minValue ;
    double maxValue ;
     //WKEInstrument *meter;
};

class biasALimitCheck:public LimitCheck
{
public:
    biasALimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
 void setMeter(WKEInstrument * meter);
private:
    double minValue ;
    double maxValue ;
    // WKEInstrument *meter;
};

class levelVLimitCheck:public LimitCheck
{
public:
    levelVLimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
 void setMeter(WKEInstrument * meter);
private:
    double minValue ;
    double maxValue ;
    // WKEInstrument *meter;
};

class levelALimitCheck:public LimitCheck
{
public:
    levelALimitCheck(WKEInstrument *meter);
    double compareMin(double value);
    double compareMax(double value);
 void setMeter(WKEInstrument * meter);
private:
    double minValue ;
    double maxValue ;
     //WKEInstrument *meter;
};



class LimitCheckFactory
{
public :
    static LimitCheck *getLimitCheckType(SweepType t, WKEInstrument *meter);
};

#endif // CLSINPUTLIMITCHECK_H
