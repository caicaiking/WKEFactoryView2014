#ifndef WKEMETERMODE_H
#define WKEMETERMODE_H

#include <QWidget>
#include "clsMeterLimit.h"
//用于meter 模式的仪表功能
enum FunctionType
{
    MeterFunction=0,UF=1
};

enum TestMode
{
    SingleStepMode=0, RepetiveMode=1
};

class WKEMeterMode : public QWidget
{
    Q_OBJECT
public:
    explicit WKEMeterMode(QWidget *parent = 0);
    virtual FunctionType getFunctionType()=0;   //获取类型
    virtual void setFunctionType(FunctionType value)=0; //设置类型
    virtual bool detectDut()=0;                 //用于探测样品
    virtual void stopDetect()=0;                //停止探测样品

    virtual void setCondition(QString value)=0; //设置测试条件
    virtual QString getConditon()=0;            //获取测试设置
    virtual void setTestMode(TestMode value)=0; //用于设置测试模式

    virtual void updateGPIB()=0;                //更新测试参数到仪器
    virtual bool getTotalStatus()=0;            //用于获取整个测试状态
    virtual int  getCountTestItems()=0;         //用于获取测试的项目数
    virtual QString getItem(int i)=0;           //用于获取测试项目（指定）
    virtual clsMeterLimit getLimit(int i)=0;    //用于获取上下限设定（指定）
    virtual void calibration()=0;               //仪表校准
    virtual QString getBrief()=0;               //获取测试步骤大概信息
    virtual QString getMeterSeries()=0;         //获取测试仪表类型
    virtual QString getDescription()=0;         //获取描述


signals:
    void biasStatus(bool);                      //更新Bias状态
    void signalTestResult(QString);             //显示单步结果
public slots:

};

#endif // WKEMETERMODE_H
