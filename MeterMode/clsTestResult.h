#ifndef CLSTESTRESULT_H
#define CLSTESTRESULT_H

#include "clsDataStore.h"
#include "WKEMeterMode.h"

struct TESTDATA_STRUCT{
    int number;
    QList <QVector <double> > data;
};

class clsTestResult: public clsDataStore
{
public:
    clsTestResult();

    int getStepCount(); //获取测试的步骤
    int getItemCount(int intStep); //获取此步骤内的items
    QList<double> getData(int intStep,
                          int intItem); //获取指定步骤的指定参数的所有数据
    clsMeterLimit getLimit(int intStep,
                           int intItem); //获取指定步骤的Limit

    QString getItem(int step, int intItem); //获取指定步骤的item
    void setMeter(WKEMeterMode *value);
    void setSteps(const QStringList value);
    void addTestData(const TESTDATA_STRUCT data);

private:

    WKEMeterMode *meter;
    QStringList steps;

    QList <TESTDATA_STRUCT> result;

};

#endif // CLSTESTRESULT_H
