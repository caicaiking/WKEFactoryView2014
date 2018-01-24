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

    bool detectDut(int threshold =0);                 //用于探测样品
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
    QString getLevel();               //获取测试的电平

    void setItemValue(SweepType t, double value);	  //用于生成一系列的测试步骤用

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
    void on_tabTestMode_currentChanged(int index);
    void on_grpFreq_toggled(bool arg1);
    void on_grpR_toggled(bool arg1);
    void on_grpC_toggled(bool arg1);
    void on_grpL_toggled(bool arg1);
    void on_btnFreqUnit_clicked();
    void on_btnRUnit_clicked();
    void on_btnCUnit_clicked();
    void on_btnLUnit_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnDepth_clicked();
    void on_btnResSpeed_clicked();
    void on_btnResEqucct_clicked();
    void lblLmItem1();
    void lblLmItem2();
    void lblLmFreq();
    void lblLmR();
    void lblLmC();
    void lblLmL();
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
    bool isStop;

    //Resonant Mode
    double freqStart, freqStop;
    QString resEqucct;
    QString resSpeed;
    int resDepth;

    clsMeterLimit mlFreq,mlR,mlC,mlL;
    QString freqSuffix,rSuffix,cSuffix,lSuffix;
    bool blFreq,blR,blC,blL;
    double res1,res2,res3,res4;

    //Select Mode
    int intSelectMode;

private:
    void updateButtons();
    double getMaxFrequency(QString value);
    QString getItemShow(const QString &item, const double &value, clsMeterLimit &limit, const QString &);
    double getMaxBiasA();
    void updateLCRGpib();
    void updateResGpib();
};

#endif // CLS6500METERMODE_H
