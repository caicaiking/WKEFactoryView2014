#ifndef CLSDATASTORE_H
#define CLSDATASTORE_H

/**!
 *
 * 主要是用于测试数据的导出的。
 *
 * */

#include <QList>
#include "clsMeterLimit.h"
#include "WKEMeterMode.h"

struct TESTDATA_STRUCT{
    int number; //测试产品的产品号
    QList <QVector <double> > data; //测试产品的数据
};

class clsDataStore
{
public:
    clsDataStore();

    virtual int getStepCount()=0; //获取测试的步骤
    virtual int getItemCount(int intStep)=0; //获取此步骤内的items
    virtual QList<double> getData(int intStep,
                                  int intItem)=0; //获取指定步骤的指定参数的所有数据
    virtual clsMeterLimit getLimit(int intStep,
                                   int intItem)=0; //获取指定步骤的Limit

    virtual QString getItem(int step, int item)=0; //获取指定步骤的item
    virtual QString getConditon(int i)=0;  //获取测试的条件
    virtual WKEMeterMode * getMeter()=0;    //获取测试仪表
    virtual int getTestCount()=0;           //获取测试的总数
    virtual TESTDATA_STRUCT getTestData(int i)=0;     //获取一个样品的测试数据
};

#endif // CLSDATASTORE_H
