#include "clsBiasAOp.h"
#include "UserfulFunctions.h"
clsBiasAOp::clsBiasAOp()
{

}

void clsBiasAOp::readSettings()
{
    clsSettings settings;

    QString strNode = "BiasASettings/";

    settings.readSetting(strNode +"PreDelayEnable",this->blPreBias);

    settings.readSetting(strNode +"PreDelayBias",this->preValue);

    settings.readSetting(strNode +"AfterDelayEnabel",this->blAfterBias);

    settings.readSetting(strNode+"AfterDelayBias",this->afterValue);

}

void clsBiasAOp::setMeter(WKEInstrument *meter)
{
    this->meter = meter;
}

void clsBiasAOp::preOperation()
{
    if(blPreBias)
        UserfulFunctions::sleepMs(preValue);
}

void clsBiasAOp::afterOperation()
{
    if(blAfterBias)
    {
        meter->turnOffBias();
        UserfulFunctions::sleepMs(afterValue);
    }
}

