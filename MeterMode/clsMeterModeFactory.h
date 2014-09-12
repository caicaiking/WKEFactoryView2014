#ifndef CLSMETERMODEFACTORY_H
#define CLSMETERMODEFACTORY_H
#include "cls6440MeterMode.h"
#include "clsUserFunctionMeterMode.h"
#include "WKEMeterMode.h"
#include <QPointer>
class clsMeterModeFactory
{
public:
    clsMeterModeFactory();

    static QPointer<WKEMeterMode>  getFunction(FunctionType t, QString meterSeries="");
};

#endif // CLSMETERMODEFACTORY_H
