#ifndef WKEMETERMODE_H
#define WKEMETERMODE_H

#include <QWidget>

//用于meter 模式的仪表功能
enum FunctionType
{
    MeterFunction=0,UF=1
};

class WKEMeterMode : public QWidget
{
    Q_OBJECT
public:
    explicit WKEMeterMode(QWidget *parent = 0);
    virtual FunctionType getType()=0;                   //获取当前步骤类型
    virtual void setCondition(const QString &value)=0;  //设置测试条件
    virtual QString getTestItem()=0;                    //获取测试的参数
    virtual void trig()=0;                              //触发测试
    virtual void start()=0;                             //重复测试
    virtual QString getTestCondition()=0;               //获取测试条件XML形式
    virtual QString getBrief()=0;                       //获取当前步骤的大概信息
    virtual bool getPassFail()=0;                       //获取测试的PASS FAil
    virtual void turnOffBias()=0;                       //关闭Bias 为了安全
signals:
    //用于单次测试
    void testResult(QString,int);
    //用于重复测试
    void startTest(QString);
public slots:

};

#endif // WKEMETERMODE_H
