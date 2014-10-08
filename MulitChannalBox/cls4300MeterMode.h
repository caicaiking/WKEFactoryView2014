#ifndef CLS4300METERMODE_H
#define CLS4300METERMODE_H

#include "ui_cls4300MeterMode.h"
#include "WKEMeterMode.h"
#include "clsMeterLimit.h"
#include "NumberInput.h"
class cls4300MeterMode : public WKEMeterMode, private Ui::cls4300MeterMode
{
    Q_OBJECT

public:
    explicit cls4300MeterMode(QWidget *parent = 0);
    FunctionType getType();
    void setCondition(const QString &value);
    QString getTestItem();
    void trig();
    void start();
    QString getTestCondition();
    QString getBrief();
    bool getPassFail();
    void updateButtons();
private slots:
    void setTest1Limit1();
    void setTest2Limit1();
    void setTest1Limit2();
    void setTest2Limit2();



    void on_btnTest1Frequency_clicked();

    void on_btnTest2Frequency_clicked();

    void on_btnTest1Level_clicked();

    void on_btnTest2Level_clicked();

    void on_btnTest1Range_clicked();

    void on_btnTest1Speed_clicked();

    void on_btnTest1Equcct_clicked();

    void on_btnTest2Equcct_clicked();

    void on_btnTest1Item1_clicked();

    void on_btnTest1Item2_clicked();

    void on_btnTest2Item1_clicked();

    void on_btnTest2Item2_clicked();

private:
    QList<clsMeterLimit> meterLimit;

    QList<double> frequency;
    QList<QString> items;
    QList<double> level;
    QList<QString> equcct;

    QString range;
    QString speed;


    double getMaxFrequency1(QString value);
};

#endif // CLS4300METERMODE_H
