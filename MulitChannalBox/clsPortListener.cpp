#include "clsPortListener.h"
#include <QtDebug>
#include <QSerialPortInfo>
#include <QString>

/*!
 * \brief clsPortListener::clsPortListener
 * 找到可用的串口，VendorId=0x403，并对串口实例化
 */
clsPortListener::clsPortListener()
{
    QString portName="";
    int vendorId=0;
    //枚举串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        vendorId =( info.hasVendorIdentifier() ? info.vendorIdentifier() :0);

        //找到设备的ID
        if(vendorId==0x403)
        {
            portName = info.portName();
            break;
        }
    }

    if(portName.isEmpty())
    {
        qDebug()<<"I can't find the serial port of venderId =403";
        this->init=false;
        return;
    }

    //初始化串口设定
    port = new QSerialPort(portName);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    this->init=  port->open(QIODevice::ReadWrite);

    if(!init)
    {
        qDebug()<<"Open Serial port failed, and the port name is: "<< portName;
        return;
    }


}


/*!
 * \brief clsPortListener::getInit
 * \return true 成功初始化，false 初始胡失败
 * 获取初始化串口状态
 */
bool clsPortListener::getInit() const
{
    return init;
}

/*!
 * \brief clsPortListener::sendCommand
 * \param value
 * \return -1 为超时，  0 为写入指令正确，  54 为写入指令错误，或者数据冲突
 * 向串口发送命令，并读取返回值
 */
int clsPortListener::sendCommand(QString value)
{
    value =value+"\r";
    /*int writeBytes= */ port->write(value.toStdString().c_str());

    //   qDebug()<<"write bytes:\t" << writeBytes;
    port->waitForBytesWritten(1000);

    if( port->waitForReadyRead(1000))
    {
        QString readValue(port->readAll());

        bool ok;
        int intRet = readValue.toInt(&ok);
        if(ok)
            return intRet;
        else
            return -1;
    }
    else
    {
        qDebug()<<"Read time out";

        return -1;
    }
}
