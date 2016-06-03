#ifndef CLSBIASAOP_H
#define CLSBIASAOP_H

#include "singleton.h"
#include "clsSettings.h"
#include "WKEInstrument.h"
class clsBiasAOp
{
public:
    clsBiasAOp();
    void readSettings();
    void setMeter(WKEInstrument * meter);
    void afterOperation();
    void preOperation();
private:
    bool blPreBias, blAfterBias;
    int preValue, afterValue;
    WKEInstrument * meter;
};

typedef Singleton<clsBiasAOp> sngBiasAOp;
#endif // CLSBIASAOP_H
