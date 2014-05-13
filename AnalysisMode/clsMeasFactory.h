#ifndef CLSMEASFACTORY_H
#define CLSMEASFACTORY_H
#include "clsMeas.h"
#include "clsTimeMeasument.h"
#include "clsFreqMeasument.h"
#include "clsBiasMeasument.h"
#include "clsBiasAMeasument.h"
class MeasFactory
{
public:
    static clsMeas* getMeas(SweepType t)
    {
        switch (t) {
        case time:
            return new clsTimeMeasument();
            break;
        case frequency:
            return new clsFreqMeasument();
            break;
        case biasV:
            return new clsBiasMeasument();
        case biasA:
            return new clsBiasAMeasument();
        default:
            break;
        }

        return (clsMeas *)0;
    }
};

#endif // CLSMEASFACTORY_H
