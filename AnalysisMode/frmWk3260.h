#ifndef CLSWK3260_H
#define CLSWK3260_H

#include "ui_frmWk3260.h"
#include "Globle.h"
#include "WKEInstrument.h"
#include "WK3260.h"
class frmWk3260 : public WKEInstrument, private Ui::frmWk3260
{
    Q_OBJECT

public:
    explicit frmWk3260(WKEInstrument *parent = 0);
    void setFrequency(double value);
    void setSpeed(QString sp);
    void setLevel(double value, QString unit);
    void setBias(double value, QString unit);
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
    void on_cmbFuction1_currentIndexChanged(const QString &arg1);
    void on_cmbBiasChioce_currentIndexChanged(int index);
    void on_btnUpdate_clicked();
    void on_btnLevel_clicked();
    void on_btnFrequency_clicked();

    void on_btnCancel_clicked();

    void setCmbboxSelected(QComboBox *cmb, QString tmp);
    void on_btnBiasValue_clicked();

    void on_btnBiasOnOFF_clicked();


protected:

private:
     WK3260 wk3260;
    bool queryBiasStatus();
    double getMaxFrequency1(QString value);
    void updateButtons();

    void readSettings(WK3260 &wk);
    void writeSettings(WK3260 wk);
};

#endif // CLSWK3260_H
