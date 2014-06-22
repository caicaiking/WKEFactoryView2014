#ifndef CLSGPIB_H
#define CLSGPIB_H
#include "connections/clsConnection.h"

/**
 *这个是用于GPIB连接的类其中包括了GPIB初始化，设置GPIB地址默认是6，发送命令和解除连接。
 *
 *这个类只能连接一台GPIB设备，多余其他的GPIB设备不能使用这个类来操作。
 *
 *这个GPIB连接类只能用于连接WK的仪器使用，因为里面限定了以“WAYNE”开头的仪器设备返回的字符串。
 *
 *作者：蔡同松
 *
 *版本：V1.0
 *
 *时间：2013-2-22
 *
 *!*/


class clsGpib:public clsConnection
{
public:
    clsGpib();
    bool init();
    void setAddress(QString address);
    QString sendCommand(QString strCommand, bool hasReturn=false, int waitDelay=0);
    void disConnect();
private:
    bool blInit;
    int intAddress;
};

#endif // CLSGPIB_H
