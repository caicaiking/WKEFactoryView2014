#ifndef CLSMETERFACOTRY_H
#define CLSMETERFACOTRY_H
#include "WKEInstrument.h"
#include "wk6500AnalysisMeter.h"
#include "wk4300AnalysisMeter.h"
class clsMeterFacotry
{
public:
    static WKEInstrument* getMeter(QString meterType)
    {
        if(meterType=="6500")
            return new wk6500AnalysisMeter();

        if(meterType == "4300")
            return new wk4300AnalysisMeter();

        return (WKEInstrument *)0;
    }
};
#endif // CLSMETERFACOTRY_H
