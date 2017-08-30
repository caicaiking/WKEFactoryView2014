#include "clsEaPs8000.h"
#include <iostream>
#include "UserfulFunctions.h"
#include <exception>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <QApplication>
#include <QDebug>
static inline float toFloat(uint8_t val[4])
{
    union
    {
        float fv;
        uint32_t value;
        uint8_t array[4];
    }b;

    b.array[3]= val[0];
    b.array[2]= val[1];
    b.array[1]= val[2];
    b.array[0]= val[3];

    //b.value = be32toh(b.value);
    return b.fv;
}

/**
 * The telegram interface to communication with PSU
 */
static int crc16 ( uint8_t *ba, int size )
{
    int work = 0;
    for ( int i = 0; i < size; i++ ) {
        work += ba[i];
    }
    return work & 0xFFFF;
}

static inline uint32_t toUint16 ( uint8_t val[2] )
{
    union
    {
        uint16_t value;
        uint8_t  array[2];
    } b;
    b.array[1] = val[0];
    b.array[0] = val[1];
    // b.value    = be16toh ( b.value );
    return b.value;
}

void clsEaPs8000::telegramStart(clsEaPs8000::SendType dir, int size)
{
    // SD
    telegram[0] = cast_type + dir + direction + ( ( size - 1 ) & 0x0F );
    // DN
    telegram[1]   = 0x01;
    telegramSize = 2;
}

void clsEaPs8000::telegramSetObject(ObjectTypes_PS object)
{
    telegram[telegramSize] = object;
    telegramSize           += 1;
}

void clsEaPs8000::pushTelegram(uint8_t value)
{
    telegram[telegramSize] = value;
    telegramSize           += 1;
}


void clsEaPs8000::openDevice()
{

    serialPort = new clsEaSerialPort();
    blInit=serialPort->openSerialPort();
}
void clsEaPs8000::telegramSend(bool /*hasReturn*/)
{

    if(telegram[0]==0)
        return;

    setTelegramCrc();


    QByteArray cmd;

    int sendCont =0;
    //int sendCommandTimes =0;


    for(int i =0; i<telegramSize; i++)
    {
        cmd.append(telegram[i]);
    }

RESEND:

    QByteArray res = serialPort->sendCommand(cmd);


    for(int i =0; i<res.length(); i++)
    {
        telegram[i] = res[i];
    }

    telegramSize = res.length();
//  //多设定电压一次
//    if(hasReturn == false)
//    {
//        sendCommandTimes ++;
//        if(sendCommandTimes< 2)
//            goto RESEND;
//    }


    if (!checkTelegramCrc ()) {
        //throw PSUError ( "Message Invalid, CRC failure" );
        //qDebug()<<"Message Invalid, CRC failure";
        sendCont++;

        //qDebug()<< cmd.toHex().data();
        if(sendCont>=4)
        {
            // qDebug()<<"Send command error!";
            return;
        }

        goto RESEND;
    }
}

void clsEaPs8000::setTelegramCrc()
{
    int16_t crc = crc16 ( telegram, telegramSize );
    telegram[telegramSize++] = ( crc >> 8 ) & 0xFF;
    telegram[telegramSize++] = ( crc ) & 0xFF;
}

bool clsEaPs8000::checkTelegramCrc()
{
    if(telegramSize == 0)
        return true;
    int16_t crc = crc16 ( telegram, telegramSize - 2 );
    if ( ( telegram[telegramSize - 2] == ( ( crc >> 8 ) & 0xFF ) ) &&
         ( telegram[telegramSize - 1] == ( ( crc ) & 0xFF ) ) )
    {
        return true;
    }
    return false;
}

void clsEaPs8000::initPower()
{
    // Retrieve nominal voltage
    telegramStart ( RECEIVE, 4 );
    telegramSetObject ( NOMINAL_VOLTAGE );
    telegramSend (true);
    nominalVoltage = toFloat ( &telegram[3] );
    // Retrieve nominal current
    telegramStart ( RECEIVE, 4 );
    telegramSetObject ( NOMINAL_CURRENT );
    telegramSend (true);
    nominalCurrent = toFloat ( &telegram[3] );
    // Retrieve nominal power
    telegramStart ( RECEIVE, 4 );
    telegramSetObject ( NOMINAL_POWER );
    telegramSend (true);
    nominalPower = toFloat ( &telegram[3] );

    // Take control over the PSU.
    // TODO: do it when only needed.

    qDebug()<< ( "\nDevice specifications:\n" );
    qDebug()<< " Nominal voltage: "<< nominalVoltage;
    qDebug()<< " Nominal current: "<< nominalCurrent;
    qDebug()<< " Nominal power:"<< nominalPower;

    this->setVoltageLimit(210.0); //For Keysight 16065A fixture spec.
    //this->setCurrentLimt(0.02);   //For Keysight 16065A fixture spec.
    this->setTestCurrent(0.02); //For Keysight 16065A fixture spec.


}

void clsEaPs8000::uninitialize()
{
    this->turnOffOutPut();
    this->setLocal();
    serialPort->closeSerialPort();
}

void clsEaPs8000::setRemote()
{
    telegramStart ( SEND, 2 );
    telegramSetObject ( POWER_SUPPLY_CONTROL );
    pushTelegram ( 0x10 );
    pushTelegram ( 0x10 );
    telegramSend ();
    telegramStart ( RECEIVE, 6 );
    telegramSetObject ( STATUS_ACTUAL );
    telegramSend (true);
}

void clsEaPs8000::setLocal()
{
    telegramStart ( SEND, 2 );
    telegramSetObject ( POWER_SUPPLY_CONTROL );
    pushTelegram ( 0x10 );
    pushTelegram ( 0x00 );
    telegramSend ();
}

void clsEaPs8000::turnOnOutput()
{
    telegramStart ( SEND, 2 );
    telegramSetObject ( POWER_SUPPLY_CONTROL );
    pushTelegram ( 0x01 );
    pushTelegram ( 0x01 );
    telegramSend ();
}

void clsEaPs8000::turnOffOutPut()
{
    telegramStart ( SEND, 2 );
    telegramSetObject ( POWER_SUPPLY_CONTROL );
    pushTelegram ( 0x01 );
    pushTelegram ( 0x00 );
    telegramSend ();
}

bool clsEaPs8000::isRemote()
{
    telegramStart ( RECEIVE, 6 );
    telegramSetObject ( STATUS_ACTUAL );
    telegramSend (true);
    return ( telegram[4] & 1 ) == 1;
}

float clsEaPs8000::getOutputCurrent()
{
    telegramStart ( RECEIVE, 6 );
    telegramSetObject ( STATUS_SET );
    telegramSend (true);
    float current = toUint16 ( &telegram[7] );
    return ( nominalCurrent * current ) / 256.0e2;
}

float clsEaPs8000::getOutputVoltage()
{
    telegramStart ( RECEIVE, 6 );
    telegramSetObject ( STATUS_SET );
    telegramSend (true);
    uint32_t voltage = toUint16 ( &telegram[3] );
    return ( nominalVoltage * voltage ) / 256.0e2;
}


float clsEaPs8000::getVoltageLimit()
{
    telegramStart ( RECEIVE, 2 );
    telegramSetObject ( OVP_THRESHOLD );
    telegramSend (true);
    float voltage = toUint16 ( &telegram[3] );
    return ( nominalVoltage * voltage ) / 256.0e2;
}

float clsEaPs8000::getCurrentLimit()
{
    telegramStart ( RECEIVE, 6 );
    telegramSetObject ( OCP_THRESHOLD );
    telegramSend (true);
    float current = toUint16 ( &telegram[3] );
    return ( nominalCurrent * current ) / 256.0e2;
}


void clsEaPs8000::setTestVoltage(float value)
{
    uint32_t val = ( value * 25600 ) / nominalVoltage;
    telegramStart ( SEND, 2 );
    telegramSetObject ( SET_VOLTAGE );
    pushTelegram ( ( val >> 8 ) & 0xFF );
    pushTelegram ( val & 0xFF );
    telegramSend ();
}

void clsEaPs8000::setTestCurrent(float value)
{
    uint32_t val = ( value * 25600 ) / nominalCurrent;
    telegramStart ( SEND, 2 );
    telegramSetObject ( SET_CURRENT );
    pushTelegram ( ( val >> 8 ) & 0xFF );
    pushTelegram ( val & 0xFF );
    telegramSend ();
}

void clsEaPs8000::setVoltageLimit(float value)
{
    uint32_t val = ( value * 25600 ) / nominalVoltage;
    telegramStart ( SEND, 2 );
    telegramSetObject ( OVP_THRESHOLD );
    pushTelegram ( ( val >> 8 ) & 0xFF );
    pushTelegram ( val & 0xFF );
    telegramSend ();
}

void clsEaPs8000::setCurrentLimt(float value)
{
    uint32_t val = ( value * 25600 ) / nominalCurrent;
    telegramStart ( SEND, 2 );
    telegramSetObject ( OCP_THRESHOLD );
    pushTelegram ( ( val >> 8 ) & 0xFF );
    pushTelegram ( val & 0xFF );
    telegramSend ();
}

void clsEaPs8000::print_device_info()
{
    qDebug()<< ( "---------------------------------------\n" );
    qDebug()<< ( "\nDevice information:\n" );
    // Get device type.
    telegramStart ( RECEIVE, 16 );
    telegramSetObject ( DEVICE_TYPE_PS );
    telegramSend (true);
    qDebug()<< " Device Type:" << toString(telegram);

    telegramStart ( RECEIVE, 16 );
    telegramSetObject ( MANUFACTURER );
    telegramSend (true);
    qDebug()<< " Manufacturer:" << toString(telegram);

    telegramStart ( RECEIVE, 16 );
    telegramSetObject ( DEVICE_ARTICLE_NO );
    telegramSend (true);
    qDebug()<< " Article No. : " << toString(telegram) ;

    telegramStart ( RECEIVE, 16 );
    telegramSetObject ( DEVICE_SERIAL_NO );
    telegramSend (true);
    qDebug()<< " Serial Num.: "<< toString(telegram);

    telegramStart ( RECEIVE, 16 );
    telegramSetObject ( SOFTWARE_VERSION );
    telegramSend (true);
    qDebug()<<" Software Version:"<< toString(telegram);
}

bool clsEaPs8000::getBlInit() const
{
    return blInit;
}

QString clsEaPs8000::toString(uint8_t x[])
{
    QString  tmpString;
    for( int i =3; i< telegramSize; i++)
    {
        if(x[i]==0)
            break;
        tmpString.append((char)x[i]);
    }
    return tmpString;
}

clsEaPs8000::clsEaPs8000() : clsPowerSupply()
{

}

bool clsEaPs8000::init()
{
    this->openDevice();
    this->print_device_info();
    this->initPower();

    return this->getBlInit();

}

void clsEaPs8000::turnON()
{
    this->setRemote();
    this->turnOnOutput();

}

void clsEaPs8000::turnOFF()
{
    this->turnOffOutPut();
    this->setLocal();

    double outVoltage;
    do
    {
        outVoltage=getOutputVoltage();
        qDebug()<<outVoltage;

        QApplication::processEvents();
    }while(outVoltage >1);
}

void clsEaPs8000::setStop()
{
    blStop =true;
}

void clsEaPs8000::setVoltage(double value)
{

    this->setTestVoltage((float)value);
    dblUserSetVoltage = value;

    blStop = false;
    QTimer *timer = new QTimer(this);
    timer->setInterval(38000);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(setStop()));
    timer->start();


    while(qAbs(getVoltage() - dblUserSetVoltage)>0.5 && (false == blStop))
    {
        UserfulFunctions::sleepMs(10);
    }
    timer->stop();
}

double clsEaPs8000::getVoltage()
{
    float actVoltage = this->getOutputVoltage();
    // qDebug()<< actVoltage;
    emit showTestValue(actVoltage);
    qApp->processEvents();
    return actVoltage;
}

void clsEaPs8000::disConnect()
{
    this->uninitialize();
}
