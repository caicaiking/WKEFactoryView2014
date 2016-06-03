#ifndef CLSMYSERIALPORT_H
#define CLSMYSERIALPORT_H

#include <QObject>
#include <QSerialPort>

/**!    //PSW 800-1.44
 *      //meas:volt:dc?
 *      //:OUTP 0
 *      //:APPLY 10.0,0.01  电压，电流
 *!*/
class clsMyserialPort
{
public:
    clsMyserialPort(QString comNumber);
    clsMyserialPort();

    ~clsMyserialPort();
    void setCom(QString value);

    QString sendCommand(QString cmmd,bool hasReturn=false);
    bool getBlInit() const;

    void sleepMs(int svalue);
    void closePort();

private:
    void initSerial();

private:
    QString comNumber;
    QString checkPorts();
    QSerialPort *serialPort;

    bool blInit;

};

#endif // CLSMYSERIALPORT_H
