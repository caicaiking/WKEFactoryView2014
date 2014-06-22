#ifndef CLSRUNINGSETTINGS_H
#define CLSRUNINGSETTINGS_H

#include <QString>

#include "clsHVRS232.h"
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

    QString instrumentModel;
    QString meterMode;
private:
    clsRS();
    clsRS(const clsRS &);
    clsRS &operator =(const clsRS &);
    virtual ~ clsRS();
    class PrivateData;
    PrivateData *data;
};

#endif // CLSRUNINGSETTINGS_H
