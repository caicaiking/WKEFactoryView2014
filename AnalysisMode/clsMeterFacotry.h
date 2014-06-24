#ifndef CLSMETERFACOTRY_H
#define CLSMETERFACOTRY_H
#include "WKEInstrument.h"
#include "wk6500AnalysisMeter.h"
#include "wk4300AnalysisMeter.h"
#include "frmWk3260.h"
#include "wk6440AnalysisMeter.h"
#include <qdebug.h>
class clsMeterFacotry
{
public:
    static WKEInstrument* getMeter(QString meterType)
    {
        //qDebug()<<meterType;
        if(meterType=="6500")
            return new wk6500AnalysisMeter();

        if(meterType == "4300")
            return new wk4300AnalysisMeter();

        if(meterType =="3260")
            return new frmWk3260();

        if(meterType =="3255")
            return new frmWk3260();

        if(meterType=="6440")
            return new wk6440AnalysisMeter();

        return (WKEInstrument *)0;
    }
};
#endif // CLSMETERFACOTRY_H
