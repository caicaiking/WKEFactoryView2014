#ifndef CLSMETERMODEFACTORY_H
#define CLSMETERMODEFACTORY_H


#include "WKEMeterMode.h"
#include <QPointer>
class clsMeterModeFactory
{
public:
    clsMeterModeFactory();

    static WKEMeterMode* getFunction(QString meterSeries="");
};

#endif // CLSMETERMODEFACTORY_H
