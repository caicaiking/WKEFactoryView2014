#ifndef CLSUSERFUNCTIONMETERMODE_H
#define CLSUSERFUNCTIONMETERMODE_H

#include "ui_clsUserFunctionMeterMode.h"
#include "WKEMeterMode.h"
class clsUserFunctionMeterMode : public WKEMeterMode, private Ui::clsUserFunctionMeterMode
{
    Q_OBJECT

public:
    explicit clsUserFunctionMeterMode(QWidget *parent = 0);
    void setCondition(const QString &value);
    QString getTestItem();
    void trig();
    void start();
    QString getTestCondition();
    QString getBrief();
    FunctionType getType();
    bool getPassFail();
    void turnOffBias();
private slots:
    void on_pushButton_clicked();
signals:
    //用于单次测试
    void testResult(QString,int);
    //用于重复测试
    void startTest(QString);
};

#endif // CLSUSERFUNCTIONMETERMODE_H
