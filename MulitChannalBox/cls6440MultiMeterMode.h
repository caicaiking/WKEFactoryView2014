#ifndef CLS6440MULTIMETERMODE_H
#define CLS6440MULTIMETERMODE_H

#include "clsMultiModeMeter.h"
#include "clsMeterLimit.h"
class cls6440MultiMeterMode : public clsMultiModeMeter
{
public:
    cls6440MultiMeterMode();

    /*01*/    void setTest1Freq();          //Freq1
    /*02*/    QString getTest1Freq();
    /*03*/    void setTest1Level();         //Level1
    /*04*/    QString getTest1Level();
    /*05*/    void setTest1Equcct();        //Equcct1
    /*06*/    QString getTest1Equcct();

    /*07*/    void setTest1Item1();         //Test1Item1
    /*08*/    QString getTest1Item1();
    /*09*/    void setTest1Item1Unit();
    /*10*/    QString getTest1Item1Unit();
    /*11*/    void setTest1Item1Limit();
    /*12*/    QString getTest1Item1Limit();

    /*13*/    void setTest1Item2Enable(bool ) ; //Test1Item2 Enable
    /*14*/    bool getTest1Item2Enable();
    /*15*/    void setTest1Item2();         //Test1Item2
    /*16*/    void setTest1Item2Unit();
    /*17*/    void setTest1Item2Limit();
    /*18*/    QString getTest1Item2();         //Test1Item2
    /*19*/    QString getTest1Item2Unit();
    /*20*/    QString getTest1Item2Limit();

    /*21*/    void setTest2On(bool );       //Test2
    /*22*/    bool getTest2On();
    /*23*/    void setTest2Freq();          //Freq2
    /*24*/    QString getTest2Freq();
    /*25*/    void setTest2Level();         //Level2
    /*26*/    QString getTest2Level();
    /*27*/    void setTest2Equcct();        //Equcct2
    /*28*/    QString getTest2Equcct();
    /*29*/    void setTest2Item2Enable(bool ) ;
    /*30*/    bool getTest2Item2Enable() ;

    /*31*/    void setTest2Item1();
    /*32*/    void setTest2Item1Unit();
    /*33*/    void setTest2Item1Limit();
    /*34*/    QString getTest2Item1();
    /*35*/    QString getTest2Item1Unit();
    /*36*/    QString getTest2Item1Limit();

    /*39*/    void setTest2Item2();
    /*40*/    void setTest2Item2Unit();
    /*41*/    void setTest2Item2Limit();
    /*42*/    QString getTest2Item2();
    /*43*/    QString getTest2Item2Unit();
    /*44*/    QString getTest2Item2Limit();

    /*45*/    void setRange() ;
    /*46*/    QString getRange();
    /*47*/    void setSpeed() ;
    /*48*/    QString getSpeed();

    /*49*/    void trig();                //触发测试
    /*50*/    int getTotleItemCount();    //获取测试总项目
    /*51*/    QString getItem(int);       //获取单个测试项目
    /*52*/    QString getUnit(int);       //获取单个测试单位
    /*53*/    double getRes(int);         //获取单个测试结果
    /*54*/    clsMeterLimit getLimit(int);      //获取单个limit
    /*55*/    bool getTotleStatus();      //获取整个测试状态
    /*55-1*/  bool getStatus(int );       //获取单个测试状态
    /*56*/    void setCondition(QString value);
    /*57*/    QString getConditon();
    void turnOnScreen(bool value);

    void setChannel(int value);   //通道测试

    void setUseLoad(bool value);  //使用Load数据
    void setConditionForCalibration(int i) ; //设置条件为了校准；
    QList<double> getOriginZA() ;  //获得原始的没有进行过校准的Z A
    void setFreqencyForCal(int value);
    double getFreqency(int value ); //获取测试的频率
private:
    double test1Freq;
    double test1LevelV;
    double test1LevelA;
    QString test1LevelType;
    QString test1Equcct;

    bool test2IsOn;
    double test2Freq;
    double test2LevelA;
    double test2LevelV;
    QString test2LevelType;
    QString test2Equcct;

    QString range;
    QString speed;

    bool isTest1Item2On;
    bool isTest2Item2On;

    QString test1Item1;
    QString test1Item1Unit;
    clsMeterLimit test1Item1Limit;
    QString test1Item2;
    QString test1Item2Unit;
    clsMeterLimit test1Item2Limit;

    QString test2Item1;
    QString test2Item1Unit;
    clsMeterLimit test2Item1Limit;
    QString test2Item2;
    QString test2Item2Unit;
    clsMeterLimit test2Item2Limit;
    int channel;
    bool useLoad;
    double test1Res1, test1Res2, test2Res1, test2Res2;

    double getMaxFreq();
    double checkFreq(double value);
    double checkLevel(double value, double freq, QString type);
    QStringList convertGpib();
    void updateGpib();
    void setFrequcy(double value);
    void setLevel(double levelV, double levelA, QString type);
};

#endif // CLS6440MULTIMETERMODE_H
