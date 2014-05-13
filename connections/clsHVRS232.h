#ifndef CLSRS232_H
#define CLSRS232_H

#include "clsPortListener.h"
#include <QTimer>
#include <QDebug>



struct NumberType
{
    int HighValue;
    int PointValue;

    double toValue()
    {
        return HighValue + PointValue*1.0/1000.0;
    }

    void setValue(QString value)
    {
        double tmp = value.toDouble();

        HighValue = int(tmp);

        PointValue =(int)((tmp-HighValue)*1000.0);
    }
};

struct VoltageMeter
{
    NumberType maxVoltage;
    NumberType maxCurrent;
    NumberType  maxPower;
    NumberType curVoltage;
    NumberType curCurrent;
    NumberType curPower;

    bool isOutPut;

    void toString()
    {
        qDebug() << "curVoltage:\t" << curVoltage.toValue();
        qDebug() << "curCurrent:\t" << curCurrent.toValue();
        qDebug() << "curPower:\t" << curPower.toValue();

        qDebug() << "maxVoltage:\t" << maxVoltage.toValue();
        qDebug() << "maxCurrent:\t" << maxCurrent.toValue();
        qDebug() << "maxPower:\t" << maxPower.toValue();
        qDebug() << "isOutput:\t" << isOutPut;

    }
};

//for rs232 connection
class clsHVRs232 :public QObject//: public clsConnection
{
    Q_OBJECT
public:
    clsHVRs232();
    bool init();
    void setAddress(QString portName);
    void disConnect();
    QString sendCommand(QString strCmmd, bool hasReturn);
    void setMaxVoltage(int value);
    VoltageMeter getMeterStatus();
    int getMaxVoltage();
    void setMaxPower(int value);
    int getMaxPower();
    void setCurVoltage(double value);
    double getCurVoltage();
    void trig(bool value=true);
public slots:
    void timeOut();
signals:
    void errorMsg(QString str);
    void showRecieve(QByteArray);
private slots:
    void readData(QByteArray res);
private:
    QString address;
    clsPortListener *serial;
    bool isInit;
    QTimer timer;
    bool isTimeOut;
    //This is for dat Recive
    QString strRecive;
    bool isFilled;
    VoltageMeter meter;
    void sleepMs(int svalue);
};


#endif // CLSRS232_H
