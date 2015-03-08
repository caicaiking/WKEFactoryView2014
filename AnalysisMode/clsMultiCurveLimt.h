#ifndef CLSMULTICURVELIMT_H
#define CLSMULTICURVELIMT_H
#include "clsMultiLimits.h"
#include <QList>
#include <QJsonDocument>
#include <QVariantMap>
#include "clsSettings.h"
class clsMultiCurveLimt
{
public:
    clsMultiCurveLimt();
    ~clsMultiCurveLimt();

    QList<clsMultiLimits> limits;
    void compareValue(double freq, double item1, double item2);
    void resetStatus();
    bool getStatus();
    bool hasEnableLimits();
    void readSettings();
    void setItem(QString item1, QString item2);
    void writeSettings();
};

#endif // CLSMULTICURVELIMT_H
