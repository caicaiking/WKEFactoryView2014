#ifndef CLSDATAPROCESS_H
#define CLSDATAPROCESS_H
#include <QString>


/**!
 * 这个默认是使用等效电路串联的，一点 惊醒开路点校准和短路点校准
 * */
#include "clsComplexOp.h"
#include "complexType.h"
class clsDataProcess
{
public:
    clsDataProcess(double z, double a, double freq);

    void setFrequency(double value);
    void setImpedance(double value);
    void setAngle(double value);
    void applyOpenData(double z, double angle);
    void applyShortData(double z,double angle);
    void applyLoadData(double z, double angle);

    void useLoadData(bool value);
    void doCalibration();
    double getItem(QString item, QString equcct);
    void applyStdData(double z, double anlge);
private:
    double mZ;
    double mA;
    double mFreq;

    bool blUseLoad;
    complexType Zo,Zs, Zsm, Zstd;

};

#endif // CLSDATAPROCESS_H
