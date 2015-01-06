#ifndef CLS6440METERMODE_H
#define CLS6440METERMODE_H

#include "ui_cls6440MeterMode.h"
#include "WKEMeterMode.h"

class cls6440MeterMode :  public WKEMeterMode , private Ui::cls6440MeterMode
{
    Q_OBJECT

public:
    explicit cls6440MeterMode(QWidget *parent = 0);
    void setCondition(const QString &value);
    QString getTestItem();
    void trig();
    void start();
    QString getTestCondition();
    QString getBrief();
    FunctionType getType();
    bool getPassFail();
    void turnOffBias();

signals:
    //用于单次测试
    void testResult(QString,int);
    //用于重复测试
    void startTest(QString);


};

#endif // CLS6440METERMODE_H
