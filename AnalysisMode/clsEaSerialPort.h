#ifndef CLSEASERIALPORT_H
#define CLSEASERIALPORT_H

#include <QObject>
#include "Qextserial/qextserialport.h"

class clsEaSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit clsEaSerialPort(QObject *parent = 0);

    bool openSerialPort();
    bool closeSerialPort();
    QByteArray sendCommand(QByteArray cmd);
    ~clsEaSerialPort(); 

protected slots:
    void readMyCom();
    void setStop();
private:
    QextSerialPort *myCom;
    bool blInit;
    QByteArray res;
    bool blStop;

private:
    void sleepMs(int svalue);
    QString checkPorts();
};

#endif // CLSEASERIALPORT_H
