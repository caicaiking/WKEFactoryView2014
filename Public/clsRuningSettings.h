#ifndef CLSRUNINGSETTINGS_H
#define CLSRUNINGSETTINGS_H

#include <QString>
#include <QStringList>
//#include "clsHVRS232.h"
class clsRS
{
public:

    static clsRS &getInst();
    //在这儿可以添加想添加的内容**********************************
    QString getConnectionType();
    void setConnectType(QString value);
    QString getIp();
    void setIp(QString value);
    void setAddress(QString value);
    QString getGpibAddress();
    void setGpibAddress(QString value);

    //连接初始化
    bool initConnection();
    void disConnect();
    QString sendCommand(QString strCommand, bool hasReturn = false);
    QString sendCommand(QString strCommand, bool hasReturn, int delayTime);
    bool isConnectionInit();
    //**********************************在这儿可以添加想添加的内容
    void lock();
    void unlock();

    QString instrumentModel; //6550 6430 6440
    QString meterSeries; //6500 4300 6440 3260 3255

    struct GPIB{
        QString NumberOfTest;
        QStringList gpibTest1; //保存仪器的状态Test1
        QStringList gpibTest2; //这个是为了4100的Test2准备的 3255 3260 6440 的RDC 模式使用
        QStringList gpibRes; //这个是为了谐振点使用
        QString biasCommand;   //bias的状态
        QString testMode;      //测试模式 AC or RDC
    } gpibCommands;

private:
    clsRS();
    clsRS(const clsRS &);
    clsRS &operator =(const clsRS &);
    virtual ~ clsRS();
    class PrivateData;
    PrivateData *data;
};

#endif // CLSRUNINGSETTINGS_H
