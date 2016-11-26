#ifndef CLSMULTIMODEMETER_H
#define CLSMULTIMODEMETER_H


#include <QObject>
#include <QString>
#include "clsMeterLimit.h"


class clsMultiModeMeter
{
public:
    clsMultiModeMeter();

    /*01*/    virtual void setTest1Freq()=0;          //Freq1
    /*02*/    virtual QString getTest1Freq()=0;
    /*03*/    virtual void setTest1Level()=0;         //Level1
    /*04*/    virtual QString getTest1Level()=0;
    /*05*/    virtual void setTest1Equcct()=0;        //Equcct1
    /*06*/    virtual QString getTest1Equcct()=0;

    /*07*/    virtual void setTest1Item1()=0;         //Test1Item1
    /*08*/    virtual QString getTest1Item1()=0;
    /*09*/    virtual void setTest1Item1Unit()=0;
    /*10*/    virtual QString getTest1Item1Unit()=0;
    /*11*/    virtual void setTest1Item1Limit()=0;
    /*12*/    virtual QString getTest1Item1Limit()=0;

    /*13*/    virtual void setTest1Item2Enable(bool ) =0; //Test1Item2 Enable
    /*14*/    virtual bool getTest1Item2Enable()=0;
    /*15*/    virtual void setTest1Item2()=0;         //Test1Item2
    /*16*/    virtual void setTest1Item2Unit()=0;
    /*17*/    virtual void setTest1Item2Limit()=0;
    /*18*/    virtual QString getTest1Item2()=0;         //Test1Item2
    /*19*/    virtual QString getTest1Item2Unit()=0;
    /*20*/    virtual QString getTest1Item2Limit()=0;

    /*21*/    virtual void setTest2On(bool )=0;       //Test2
    /*22*/    virtual bool getTest2On()=0;
    /*23*/    virtual void setTest2Freq()=0;          //Freq2
    /*24*/    virtual QString getTest2Freq()=0;
    /*25*/    virtual void setTest2Level()=0;         //Level2
    /*26*/    virtual QString getTest2Level()=0;
    /*27*/    virtual void setTest2Equcct()=0;        //Equcct2
    /*28*/    virtual QString getTest2Equcct()=0;
    /*29*/    virtual void setTest2Item2Enable(bool ) =0;
    /*30*/    virtual bool getTest2Item2Enable() =0;

    /*31*/    virtual void setTest2Item1()=0;
    /*32*/    virtual void setTest2Item1Unit()=0;
    /*33*/    virtual void setTest2Item1Limit()=0;
    /*34*/    virtual QString getTest2Item1()=0;
    /*35*/    virtual QString getTest2Item1Unit()=0;
    /*36*/    virtual QString getTest2Item1Limit()=0;

    /*39*/    virtual void setTest2Item2()=0;
    /*40*/    virtual void setTest2Item2Unit()=0;
    /*41*/    virtual void setTest2Item2Limit()=0;
    /*42*/    virtual QString getTest2Item2()=0;
    /*43*/    virtual QString getTest2Item2Unit()=0;
    /*44*/    virtual QString getTest2Item2Limit()=0;

    /*45*/    virtual void setRange() =0;
    /*46*/    virtual QString getRange()=0;
    /*47*/    virtual void setSpeed() =0;
    /*48*/    virtual QString getSpeed()=0;

    /*49*/    virtual void trig()=0;                //触发测试
    /*50*/    virtual int getTotleItemCount()=0;    //获取测试总项目
    /*51*/    virtual QString getItem(int)=0;       //获取单个测试项目
    /*52*/    virtual QString getUnit(int)=0;       //获取单个测试单位
    /*53*/    virtual double getRes(int)=0;         //获取单个测试结果
    /*54*/    virtual clsMeterLimit getLimit(int)=0;      //获取单个limit
    /*55*/    virtual bool getTotleStatus()=0;      //获取整个测试状态
    /*55-1*/         virtual bool getStatus(int )=0;       //获取单个测试状态
    /*56*/    virtual void setCondition(QString json)=0;
    /*57*/    virtual QString getConditon()=0;


    virtual void setChannel(int value)=0;   //通道测试
    virtual void setUseLoad(bool value)=0;  //使用Load数据
    virtual void setConditionForCalibration(int)=0; //设置条件为了校准；
    virtual QList<double> getOriginZA()=0;  //获得原始的没有进行过校准的Z A
    virtual void setFreqencyForCal(int)=0; //设置频率，为了校准
    virtual double getFreqency(int value )=0; //获取测试的频率
    virtual void turnOnScreen(bool value){} //关闭测试屏幕
};

#endif // CLSMULTIMODEMETER_H
