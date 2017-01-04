#ifndef CLSPOWERSUPPLY
#define CLSPOWERSUPPLY
#include <QObject>
class clsPowerSupply :public QObject
{
    Q_OBJECT
public:
    clsPowerSupply();
    virtual bool init()=0;
    virtual void turnON()=0;
    virtual void turnOFF()=0;
    virtual void setVoltage(double value)=0;
    virtual double getVoltage()=0;
    virtual void disConnect()=0;

signals:
    void showTestValue(double value);
};

#endif // CLSPOWERSUPPLY
