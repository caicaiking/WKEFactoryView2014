#ifndef WK6440ANALYSISMETER_H
#define WK6440ANALYSISMETER_H

#include "ui_wk6440AnalysisMeter.h"
#include "WKEInstrument.h"
class wk6440AnalysisMeter : public WKEInstrument, private Ui::wk6440AnalysisMeter
{
    Q_OBJECT

public:
    explicit wk6440AnalysisMeter(WKEInstrument *parent = 0);


    void setFrequency(double value);
    void setSpeed(QString);
    void setLevel(double value, QString unit);
    void setBias(double, QString);
    void getFrequency(double * value);
    void getLevel(double *value, QString *unit);
    void getBias(double *value, QString *unit);

    void readSettings();      //读取设定
    void saveSettings();      //保存设置
    bool turnOffBias();       //关闭Bias 为了安全
    bool turnOnBias();         //打开Bias
    QString trig();           //触发测量，返回字符创
    void calibration();
    void updateInstrument();  // 更新仪器设定

    QString getItem1();
    QString getItem2();
    QString getEqucct();
    double getMaxBiasV();
    double getMinBiasV();
    double getMaxBiasA();
    double getMinBiasA();
    double getMaxLevelA();
    double getMinLevelA();
    double getMaxLevelV();
    double getMinLevelV();
    double getMaxFrequency();
    double getMinFrequency();

    QString getSuportFunction();
signals:
    void sweepItemSignal(QString,QString);  //扫描的参数
    void levelSignal(QString);              //返回电平
    void speedSignal(QString);              //返回速度
    void frequencySignal(QString);          //返回测试频率
    void biasStatusSignal(bool);            //显示Bias状态
    void biasValueSignal(QString);          //返回Bias的值

private slots:


    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_btnItem1_clicked();

    void on_btnItem2_clicked();

    void on_btnEqucct_clicked();

    void on_btnRange_clicked();

    void on_btnSpeed_clicked();

    void on_btnFrequency_clicked();

    void on_btnLevel_clicked();

    void on_btnBiasSource_clicked();

    void on_btnBiasStatus_clicked();

private:
    QString item1;
    QString item2;
    QString range;
    QString speed;
    double frequency;
    double levelVValue;
    double levelAValue;
    QString levelType;

    double biasVValue;
    double biasAValue;
    QString biasType;
    bool biasONOFF;
    QString equcct;

    bool queryBiasStatus();
    double getMaxFrequency1(QString value);
    void updateButtons();
};

#endif // WK6440ANALYSISMETER_H
