#include "clsGwPsw800.h"
#include <QDebug>
clsGwPsw800::clsGwPsw800()
{
    serialPort = new clsMyserialPort();
}

bool clsGwPsw800::init()
{
    return serialPort->getBlInit();
}

void clsGwPsw800::turnON()
{
    serialPort->sendCommand(":OUTP 1",false);
}

void clsGwPsw800::turnOFF()
{
    serialPort->sendCommand(":OUTP 0",false);
}

void clsGwPsw800::setVoltage(double value)
{
    QString setVoltageCommand = QString(":APPLY %1,0.01").arg(value);
    serialPort->sendCommand(setVoltageCommand,false);

    int i=0;
    while(i<50)
    {
        i++;
        serialPort->sleepMs(400);

        if(qAbs(getVoltage() - value)<1)
        {
            qDebug()<<"Finish!";
            break;
        }
    }

}

double clsGwPsw800::getVoltage()
{
    QString returnValue =serialPort->sendCommand("meas:volt:dc?",true);
    return returnValue.toDouble();
}

void clsGwPsw800::disConnect()
{
    serialPort->closePort();
}

