#ifndef WKEMETERMODE_H
#define WKEMETERMODE_H

#include <QWidget>
#include "clsMeterLimit.h"
//用于meter 模式的仪表功能
enum FunctionType
{
    MeterFunction=0,UF=1
};


class clsAdditionOP
{

public:
    virtual void doOperation()=0;

    void setConditon(QString value)
    {
        this->conditons = value;
    }

    QString getConditon()
    {
        return this->conditons;
    }

private:
    QString conditons;
};

class WKEMeterMode : public QWidget
{
    Q_OBJECT
public:
    explicit WKEMeterMode(QWidget *parent = 0);

    virtual bool detectDut()=0;                 //用于探测样品
    virtual void stopDetect()=0;                //停止探测样品
    virtual void setCondition(QString value)=0; //设置测试条件
    virtual QString getConditon()=0;            //获取测试设置
    virtual void updateGPIB()=0;                //更新测试参数到仪器
    virtual bool getTotalStatus()=0;            //用于获取整个测试状态
    virtual int  getCountTestItems()=0;         //用于获取测试的项目数
    virtual QString getItem(int i)=0;           //用于获取测试项目（指定）
    virtual clsMeterLimit getLimit(int i)=0;    //用于获取上下限设定（指定）
    virtual double getResult(int i)=0;          //获取测试数据（指定）
    virtual QString getSuffix(int i)=0;         //获取显示数据的后缀（指定）
    virtual void calibration()=0;               //仪表校准
    virtual QString getBrief()=0;               //获取测试步骤大概信息
    virtual QString getMeterSeries()=0;         //获取测试仪表类型
    virtual QString getDescription()=0;         //获取描述
    virtual void turnOffBias()=0;               //关闭Bias
    virtual void singleTrig()=0;                //单次测试
    virtual void repetiveTrig()=0;              //重复测试模式
    virtual QString getFreq()=0;                //获取测试频率
    virtual QString getEqucct()=0;              //获取测试等效电路
    virtual QString getLevel()=0;               //获取测试的电平
    virtual QString getBias(){return "";}       //获取Bias值

    void addSomeAdditionOperation(clsAdditionOP *op)
    {
        if(op !=NULL)
            op->doOperation();
    }

signals:
    void biasStatus(bool);                      //更新Bias状态
    void signalTestResult(QString);             //显示单步结果
    void detectInProgress(QString);
public slots:

};

#endif // WKEMETERMODE_H
