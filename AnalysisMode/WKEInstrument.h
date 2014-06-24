#ifndef WKEINSTRUMENT_H
#define WKEINSTRUMENT_H

#include <QDialog>

/*!
 * \brief The WKEInstrument class
 * WK扫描模式的仪器参数的窗口。
 * 以后不论是添加哪种仪器都要继承这样的窗口虚函数。
 *
 * 如果是6500的话 采用的是仪表模式是为了通用性。
 * 43（41）00 采用 测量模式
 * 64（63）00 采用 测量模式
 * 3260 采用阻抗模式
 * 3255 采用测试模式
 *
 */
class WKEInstrument : public QDialog
{
    Q_OBJECT
public:
    explicit WKEInstrument(QDialog *parent = 0);

    virtual void setFrequency(double value)=0;  //设定频率
    virtual void setSpeed(QString sp)=0;        //设定速度
    virtual void setLevel(double value, QString unit)=0;    //设定电平
    virtual void setBias(double value, QString unit)=0;
    virtual void getFrequency(double * value)=0;
    virtual void getLevel(double *value, QString *unit)=0;
    virtual void getBias(double *value, QString *unit)=0;

    virtual void readSettings()=0;      //读取设定
    virtual void saveSettings()=0;      //保存设置
    virtual bool turnOffBias()=0;       //关闭Bias 为了安全
    virtual bool turnOnBias()=0;        //打开Bias 设置
    virtual QString trig()=0;           //触发测量，返回字符创
    virtual void updateInstrument()=0;  // 更新仪器设定

    virtual QString getItem1()=0;       //返回测试项目1
    virtual QString getItem2()=0;       //返回测试项目2
    virtual QString getEqucct()=0;

    virtual double getMaxFrequency()=0;         //返回仪表的最大测试频率
    virtual double getMinFrequency()=0;
    virtual double getMaxBiasV()=0;
    virtual double getMinBiasV()=0;
    virtual double getMaxBiasA()=0;
    virtual double getMinBiasA()=0;
    virtual double getMaxLevelA()=0;
    virtual double getMinLevelA()=0;
    virtual double getMaxLevelV()=0;
    virtual double getMinLevelV()=0;
    virtual void calibration()=0;
    virtual QString getSuportFunction()=0;  //支持的扫描类型
signals:
    virtual void sweepItemSignal(QString,QString)=0;  //扫描的参数
    virtual void levelSignal(QString)=0;       //返回电平
    virtual void speedSignal(QString)=0;              //返回速度
    virtual void frequencySignal(QString)=0;          //返回测试频率
    virtual void biasStatusSignal(bool)=0;            //显示Bias状态
    virtual void biasValueSignal(QString)=0;
public slots:

};



#endif // WKEINSTRUMENT_H
