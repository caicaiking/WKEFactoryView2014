#include "clsConnectSWBox.h"
#include <QMutex>
#include <QObject>
#include <QApplication>
#include <QDebug>
//将_instance 指针初始化为0
clsConnectSWBox *clsConnectSWBox::_instance = 0;

clsConnectSWBox::clsConnectSWBox(QObject *parent) :
    QObject(parent)
{
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
            _instance = new clsConnectSWBox;
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
