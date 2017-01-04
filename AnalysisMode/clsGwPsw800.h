#ifndef CLSGWPSW800_H
#define CLSGWPSW800_H

#include "clsPowerSupply.h"
#include "clsMyserialPort.h"
class clsGwPsw800 : public clsPowerSupply
{
      Q_OBJECT
public:


    bool init();
    void turnON();
    void turnOFF();
    void setVoltage(double value);
    double getVoltage();
    void disConnect();

private:
    clsMyserialPort *serialPort;
signals:
    void showTestValue(double value);
};

#endif // CLSGWPSW800_H
