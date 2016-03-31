#ifndef CLSPOWERSUPPLY
#define CLSPOWERSUPPLY
class clsPowerSupply
{
public:
    virtual bool init()=0;
    virtual void turnON()=0;
    virtual void turnOFF()=0;
    virtual void setVoltage(double value)=0;
    virtual double getVoltage()=0;
    virtual void disConnect()=0;
};

#endif // CLSPOWERSUPPLY

