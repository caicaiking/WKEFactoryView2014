#ifndef CLS6500METERMODE_H
#define CLS6500METERMODE_H

#include "ui_cls6500MeterMode.h"
#include "WKEMeterMode.h"
#include "clsMeterLimit.h"
class cls6500MeterMode : public WKEMeterMode, private Ui::cls6500MeterMode
{
    Q_OBJECT

public:
    explicit cls6500MeterMode(WKEMeterMode *parent = 0);

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
private slots:
    void on_btnRange_clicked();
    void on_btnSpeed_clicked();
    void on_btnEqucct_clicked();
    void on_btnFrequency_clicked();
    void on_btnLevel_clicked();
    void on_btnBiasLevel_clicked();
    void on_btnBiasType_clicked();
    void on_btnBiasStatus_clicked();
    void on_btnItem1_clicked();
    void on_btnItem2_clicked();
    void on_btnItem1Unit_clicked();
    void on_btnItem2Unit_clicked();

    void lblLmItem1();
    void lblLmItem2();
private:
    QString range,speed,equcct;
    QString levelType;
    double levelAValue,levelVValue;
    double frequency;
    QString item1,item2;
    double biasVValue, biasAValue;
    QString biasType,strBiasStatus;
    QString item1Suffix,item2Suffix;
    clsMeterLimit mlItem1,mlItem2;
    double item1Res, item2Res;
    void updateButtons();
    double getMaxFrequency(QString value);
    QString getItemShow(const QString &item, const double &value, clsMeterLimit &limit, const QString &);
    bool isStop;
    double getMaxBiasA();
};

#endif // CLS6500METERMODE_H
