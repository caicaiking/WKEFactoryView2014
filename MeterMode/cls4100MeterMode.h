#ifndef CLS4100METERMODE_H
#define CLS4100METERMODE_H

#include "ui_cls4100MeterMode.h"
#include "WKEMeterMode.h"
#include "clsMeterLimit.h"
class cls4100MeterMode : public WKEMeterMode, private Ui::cls4100MeterMode
{
    Q_OBJECT

public:
    explicit cls4100MeterMode(QWidget *parent = 0);

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
    void on_btnTest1Frequency_clicked();
    void on_btnTest1Level_clicked();
    void on_btnTest1Range_clicked();
    void on_btnTest1Speed_clicked();
    void on_btnTest1Equcct_clicked();
    void on_btnTest1Item1_clicked();
    void on_btnTest1Item1Unit_clicked();
    void on_btnTest1Item2_clicked();
    void on_btnTest1Item2Unit_clicked();
    void on_btnTest2Level_clicked();
    void on_btnTest2Frequency_clicked();
    void on_btnTest2Equcct_clicked();
    void on_btnTest2Item1_clicked();
    void on_btnTest2Item1Unit_clicked();
    void on_btnTest2Item2_clicked();
    void on_btnTest2Item2Unit_clicked();
    void btnTest1Limit1clicked();
    void btnTest1Limit2clicked();
    void btnTest2Limit1clicked();
    void btnTest2Limit2clicked();
private:

    QString test1Item1,test1Item2,test2Item1,test2Item2;
    QString item1Suffix,item2Suffix,item3Suffix,item4Suffix;
    clsMeterLimit mlItem1,mlItem2,mlItem3,mlItem4;
    double levelTest1Ac, levelTest1Rdc, levelTest2Ac, levelTest2Rdc;
    double test1Freq,test2Freq;
    QString test1Equcct,test2Equcct;
    QString test1Range, test1Speed;
    double item1Res,item2Res,item3Res,item4Res;
    void updateButtons();
    double getMaxFrequency(QString value);
    QString getItemShow(const QString &item, const double &value, clsMeterLimit &limit, const QString &);
    QStringList convertTest2Gpib();
    QStringList convertTest1Gpib();
    bool isStop;
};

#endif // CLS4100METERMODE_H
