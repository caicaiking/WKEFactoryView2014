#ifndef CLSEAPS8000_H
#define CLSEAPS8000_H


#include <stdint.h>
#include "clsEaSerialPort.h"
#include "clsPowerSupply.h"
class clsEaPs8000 : public clsPowerSupply
{
      Q_OBJECT
public:
        clsEaPs8000();
      bool init();
      void turnON();
      void turnOFF();
      void setVoltage(double value);
      double getVoltage();
      void disConnect();


    void initPower();
    void uninitialize();
    void openDevice();
    void setRemote();
    void setLocal();
    void turnOnOutput();
    void turnOffOutPut();
    bool isRemote();
    float getOutputCurrent();
    float getOutputVoltage ();
    float getVoltageLimit () ;
    float getCurrentLimit () ;
    void setTestVoltage ( float value );
    void setTestCurrent ( float value );
    void setVoltageLimit ( float value );
    void setCurrentLimt ( float value );
    void print_device_info ();

    bool getBlInit() const;

private:
    bool blInit;
    bool blStop;
    clsEaSerialPort *serialPort;
    enum ErrorTypes
    {
        NO_ERRORS              = 0x0,
        CRC_INVALID           = 0x3,
        DELIMITER_INVALID     = 0x4,
        OUTPUT_ADDR_INVALID   = 0x5,
        OBJECT_INVALID        = 0x7,
        OBJECT_LENGTH_INVALID = 0x8,
        ACCESS_VIOLATION      = 0x9,
        DEVICE_LOCKED         = 0x15,
        OBJECT_OVERFLOW       = 0x30,
        OBJECT_UNDERFLOW      = 0x31
    };

    struct
    {
        ErrorTypes type;
        const char * name;
    }
    ErrorTypeStr[10]={
    { NO_ERRORS,              "No error"                                   },
    { CRC_INVALID,           "Check sum incorrect"                        },
    { DELIMITER_INVALID,     "Start delimiter incorrect"                  },
    { OUTPUT_ADDR_INVALID,   "Wrong address for output"                   },
    { OBJECT_INVALID,        "Object not defined"                         },
    { OBJECT_LENGTH_INVALID, "Object length incorrect"                    },
    { ACCESS_VIOLATION,      "Read/Write permissions violated, no access" },
    { DEVICE_LOCKED,         "Device is in \"Lock\" state"                },
    { OBJECT_OVERFLOW,       "Upper limit of object exceeded"             }
};
    enum ObjectTypes_PS
    {
        DEVICE_TYPE_PS          = 0,
        DEVICE_SERIAL_NO     = 1,
        NOMINAL_VOLTAGE      = 2,
        NOMINAL_CURRENT      = 3,
        NOMINAL_POWER        = 4,
        DEVICE_ARTICLE_NO    = 6,
        MANUFACTURER         = 8,
        SOFTWARE_VERSION     = 9,
        DEVICE_CLASS         = 19,
        OVP_THRESHOLD        = 38,
        OCP_THRESHOLD        = 39,
        SET_VOLTAGE          = 50,
        SET_CURRENT          = 51,
        POWER_SUPPLY_CONTROL = 54,
        STATUS_ACTUAL        = 70,
        STATUS_SET           = 71,
    };

    float nominalVoltage =1;
    float nominalCurrent =1;
    float nominalPower =1;

    //max length is SD(1) + DN(1) + Obj(2) + Data(0-16) + CS(2)

    uint8_t telegram[22]={0,};
    uint8_t telegramSize =0;

    enum SendType  //SD 7 6
    {
        SEND   =0xC0,
        RECEIVE=0x40,/*0x80*/
    };

    const int direction =0x10; //传播方向 SD 5
    const int cast_type =0x00; //广播类型 SD 4

    double dblUserSetVoltage;
private:
    void telegramStart(SendType dir, int size);
    void telegramSetObject(ObjectTypes_PS object);
    void pushTelegram(uint8_t value);
    void telegramSend(bool hasReturn=false);
    void setTelegramCrc();
    bool checkTelegramCrc();
    QString toString(uint8_t x[]);
signals:
    void showTestValue(double value);
protected slots:
    void setStop();
};

#endif // CLSEAPS8000_H
