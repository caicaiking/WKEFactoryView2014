#ifndef CLSADDITIONOPFACTORY_H
#define CLSADDITIONOPFACTORY_H
#include "clsMeterMode.h"
#include "cls6500TurnOffScreen.h"
#include "cls6440TurnOffScreen.h"
class clsAddtionOpFactory
{
public:
    static clsAdditionOP * getAddtionOp(QString meterSeries)
    {
        if(meterSeries =="6500")
            return new cls6500TurnOffScreen();
        else if(meterSeries == "6440")
            return new cls6440TurnOffScreen();
        else
            return (clsAdditionOP*)(0);
    }
};

#endif // CLSADDITIONOPFACTORY_H
