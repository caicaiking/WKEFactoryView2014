#ifndef CLSUSB_H
#define CLSUSB_H
#include "connections/visa.h"
#include "connections/clsConnection.h"
/**
 *这个是用于USB连接的类其中包括了USB初始化，通过查询字符串来寻找测试仪器，发送命令和解除连接。
 *
 *这个类只能连接一台USB设备，多余其他的USB设备不能使用这个类来操作。
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
#define MAX_CNT 500

class clsUsb : public clsConnection
{
public:
    clsUsb();
    bool init();
    void setAddress(QString address);
    QString sendCommand(QString strCommand, bool hasReturn=false, int waitDelay=5);
    void disConnect();
private:
    bool blInit;

    ViStatus status;            /* For checking errors  */
    ViSession defaultRM, instr; /* Communication channels  */
    ViUInt32 retCount;          /* Return count from string I/O */
    ViChar desc[MAX_CNT];       /* Buffer for string I/O  */
    ViUInt32   numInstrs;       /* The number of instruments */
    ViFindList findList;        /* find lists of instrument */
    ViUInt32 retLen;           /* The return length  */
    unsigned char buffer[MAX_CNT];       /* For read result back */
    ViByte command[];
    QString strResult;
};

#endif // CLSUSB_H
