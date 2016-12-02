#ifndef CLSSINGLETRIG_H
#define CLSSINGLETRIG_H
#include <QString>
#include "clsDataProcess.h"
class clsSingleTrig
{
public:
    clsSingleTrig();

    int getChannel() const;
    void setChannel(int value);

    double getFrequency() const;
    void setFrequency(double value);

    double getZm() const;
    void setZm(double value);

    double getAm() const;
    void setAm(double value);

    bool getBlUseLoad() const;
    void setBlUseLoad(bool value);

    void doLoadCalibration();
    void doRCCalibration();

    double getItem(QString item, QString equcct);

private:
    int channel;
    double frequency;
    double Zm;
    double Am;
    bool blUseLoad;

    clsDataProcess *dp;

};

#endif // CLSSINGLETRIG_H
