#ifndef CLSMEASMENTSETUP_H
#define CLSMEASMENTSETUP_H

class clsMeasurementSetup
{
public :
    virtual bool readSettings();
    virtual void writeSettings();
    virtual bool transforSettings();
};

#endif // CLSMEASMENTSETUP_H
