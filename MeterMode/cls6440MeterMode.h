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

    FunctionType getFunctionType();   //获取类型
    void setFunctionType(FunctionType value); //设置类型
    bool detectDut();                 //用于探测样品
    void stopDetect();                //停止探测样品

    void setCondition(QString value); //设置测试条件
    QString getConditon();            //获取测试设置
    void setTestMode(TestMode value); //用于设置测试模式

    void updateGPIB();                //更新测试参数到仪器
    bool getTotalStatus();            //用于获取整个测试状态
    int  getCountTestItems();         //用于获取测试的项目数
    QString getItem(int i);           //用于获取测试项目（指定）
    clsMeterLimit getLimit(int i);    //用于获取上下限设定（指定）
    void calibration();               //仪表校准
    QString getBrief();               //获取测试步骤大概信息
    QString getMeterSeries();         //获取测试仪表信息
    QString getDescription();         //获取描述信息
signals:

private slots:
    void limitMajorClick();
    void limitMinorClick();
    void limitRdcClick();
    void updateButtons();
    void on_btnMajorUnit_clicked();

    void on_btnMinorUnit_clicked();

    void on_btnRdcUnit_clicked();

private:
    QString strDescription;

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

    clsMeterLimit lmMajor,lmMinor,lmRdc;

    QString majorUnit,minorUnit,rdcUnit;





};

#endif // CLS6440METERMODE_H
