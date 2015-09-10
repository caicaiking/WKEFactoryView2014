#ifndef CLS6440METERMODE_H
#define CLS6440METERMODE_H

#include "ui_cls6440MeterMode.h"
#include "WKEMeterMode.h"
#include "clsMeterLimit.h"
class cls6440MeterMode :  public WKEMeterMode, private Ui::cls6440MeterMode
{
    Q_OBJECT

public:
    explicit cls6440MeterMode(WKEMeterMode *parent = 0);


    bool detectDut();                 //用于探测样品
    void stopDetect();                //停止探测样品
    void setCondition(QString value); //设置测试条件
    QString getConditon();            //获取测试设置
    void updateGPIB();                //更新测试参数到仪器
    bool getTotalStatus();            //用于获取整个测试状态
    int  getCountTestItems();         //用于获取测试的项目数
    QString getItem(int i);           //用于获取测试项目（指定）
    clsMeterLimit getLimit(int i);    //用于获取上下限设定（指定）
    void calibration();               //仪表校准
    QString getBrief();               //获取测试步骤大概信息
    QString getMeterSeries();         //获取测试仪表信息
    QString getDescription();         //获取描述信息
    void turnOffBias();               //关闭Bias
    void singleTrig();                //单次测试
    void repetiveTrig();              //重复测试模式
    double getResult(int i);          //获取测试数据
    QString getSuffix(int i);         //获取显示数据的后缀
    QString getFreq();                //获取测试频率
    QString getEqucct();              //获取测试等效电路
signals:

private slots:
    void limitMajorClick();
    void limitMinorClick();
    void limitRdcClick();
    void updateButtons();
    void on_btnMajorUnit_clicked();
    void on_btnMinorUnit_clicked();
    void on_btnRdcUnit_clicked();
    void on_btnFrequency_clicked();
    double getMaxFreq();
    void on_btnLevel_clicked();
    double getOkLevel(double value, QString unit);
    void on_btnEqucct_clicked();
    void on_btnRange_clicked();
    void on_btnSpeed_clicked();
    void on_btnBiasSource_clicked();
    void on_btnBiasStatus_clicked();
    void on_btnItem1_clicked();
    void on_btnItem2_clicked();
    void on_btnRdcRange_clicked();
    void on_btnRdcSpeed_clicked();
    void on_btnRdcLevel_clicked();

    void on_txtDescription_textChanged(const QString &arg1);


    void on_grpMinor_clicked(bool checked);

private:
    QString strDescription;
    bool enableMinor;
    QString item1;
    QString item2;
    double level;
    QString levelType;
    double frequency;
    QString biasSource;
    bool blBiasStatus;
    QString range;
    QString speed;
    QString equcct;
    double levelRdc;
    QString rangeRdc;

    clsMeterLimit lmMajor,lmMinor,lmRdc;

    QString majorUnit,minorUnit,rdcUnit;



    double dblItem1, dblItem2, dblRdc;

    bool blStop;


    QString getItemShow(const QString &item, const double &value, clsMeterLimit &limit, const QString &suffix);
};

#endif // CLS6440METERMODE_H
