#ifndef CLSMEASFACTORY_H
#define CLSMEASFACTORY_H
#include "clsMeas.h"
#include "clsTimeMeasument.h"
#include "clsFreqMeasument.h"
#include "clsBiasMeasument.h"
#include "clsBiasAMeasument.h"
#include "clsBiasExtMeasument.h"
class MeasFactory
{
public:
    static clsMeas* getMeas(SweepType t)
    {
        switch (t) {
        case Time:
            return new clsTimeMeasument();
            break;
        case Frequency:
            return new clsFreqMeasument();
            break;
        case BiasV:
            return new clsBiasMeasument();
        case BiasExtV:
            return new clsBiasExtMeasument();
        case BiasA:
            return new clsBiasAMeasument();
        default:
            break;
        }

        return (clsMeas *)0;
    }
};

#endif // CLSMEASFACTORY_H
