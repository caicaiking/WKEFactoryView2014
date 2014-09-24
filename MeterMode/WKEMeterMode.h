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
    virtual FunctionType getType()=0;
    virtual void setCondition(const QString &value)=0;
    virtual QString getTestItem()=0;
    virtual void trig()=0;
    virtual void start()=0;
    virtual QString getTestCondition()=0;
    virtual QString getBrief()=0;
    virtual bool getPassFail()=0;
signals:
    //用于单次测试
    void testResult(QString,int);
    //用于重复测试
    void startTest(QString);
public slots:

};

#endif // WKEMETERMODE_H