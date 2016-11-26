#include "clsConnectSWBox.h"
#include <QMutex>
#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <math.h>
using namespace std;
//将_instance 指针初始化为0
clsConnectSWBox *clsConnectSWBox::_instance = 0;

clsConnectSWBox::clsConnectSWBox(QObject *parent) :
    QObject(parent)
{

    chennals.clear();
    QString tmp;

    for(int i=0; i<16; i++)
    {
        int value=  pow(2,i);
        tmp = QString("4,%1,0").arg(QString::number(value));
        chennals<<tmp;
    }

    for(int i=1; i<5; i++)
    {
        int value=  pow(2,i-1);
        tmp = QString("4,0,%1").arg(QString::number(value));
        chennals<<tmp;
    }
}

/*!
 * \brief clsConnectSWBox::Instance
 * \return _instance 指针
 * 实例化自身
 */
clsConnectSWBox *clsConnectSWBox::Instance()
{
    static QMutex mutex;
    if (!_instance) {
        QMutexLocker locker(&mutex);
        if (!_instance) {
            _instance = new clsConnectSWBox();
        }
    }
    return _instance;
}

/*!
 * \brief clsConnectSWBox::initSerialPort
 * \param portAddress 端口地址
 * \return true 初始化成功 false 初始化失败
 * 初始化串口程序
 */
bool clsConnectSWBox::initSerialPort()
{
    serialPort = new clsPortListener();
    return serialPort->getInit();
}

void clsConnectSWBox::closeSeriesPort()
{
    serialPort->closePort();
}

/*!
 * \brief clsConnectSWBox::sendCommand
 * \param value 命令内容
 * \return -1 发送超时 0 发送成功 54 发送有问题
 * 发送命令
 */
int clsConnectSWBox::sendCommand(QString value)
{
    return   serialPort->sendCommand(value);
}

using namespace std;

int clsConnectSWBox::selectChannel(int chennal)
{
    return sendCommand(QString("4,%1").arg(chennal));
}

int clsConnectSWBox::setChannelStatus(int channel, Status status)
{
    if(isLedOff)
        return 0;
    switch (status) {
    case PASS:
       return sendCommand(QString("6,%1").arg(channel));
        break;
    case FAIL:
         return sendCommand(QString("7,%1").arg(channel));
        break;
    case IDEL:
         return sendCommand(QString("8,%1").arg(channel));
        break;
    default:
        break;
    }
    return 0;
}

int clsConnectSWBox::setAllChannelIdel()
{
    if(isLedOff)
        return 0;

    return sendCommand(QString("9"));
}

int clsConnectSWBox::setTerminal(int value)
{
    return sendCommand(QString("11,%1").arg(value));
}

int clsConnectSWBox::turnOffAllLight()
{
    if(isLedOff)
        return 0;
    return sendCommand(QString("10"));
}

int clsConnectSWBox::setOnlyOneOrangeLEDON(int channel)
{
    //this->turnOffAllLight();
    if(isLedOff)
        return 0;

    return this->setChannelStatus(channel,IDEL);
}

void clsConnectSWBox::setLedOff(bool value)
{
    this->isLedOff = value;

    if(value == true)
        sendCommand("10");
}
