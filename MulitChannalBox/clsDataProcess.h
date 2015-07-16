#ifndef CLSDATAPROCESS_H
#define CLSDATAPROCESS_H


/**!
 * ���Ĭ����ʹ�õ�Ч��·������
 * */
#include "clsComplexOp.h"

class clsDataProcess
{
public:
    clsDataProcess(double z, double a, double freq);

    void setFrequency(double value);
    void setImpedance(double value);
    void setAngle(double value);
    void applyOpenData(double z, double angle);
    void applyShortData(double z,double angle);

    void calculate();
private:
    double mZ;
    double mA;
    double mFreq;

    CPLXTrig openData,shortData;

};

#endif // CLSDATAPROCESS_H
