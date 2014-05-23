#ifndef GLOBLE_H
#define GLOBLE_H

#include <QString>
#include "doubleType.h"
#include <QObject>
#include <QList>

enum SweepType
{
    Time=0,Frequency=1,BiasV=2,BiasA=3,levelV=4,levelA=5
};

/*!

*/

struct graphSetup
{
    double xmin;
    double xmax;
    bool logX;

    double yLeftMin;
    double yLeftMax;
    bool logYleft;

    double yRightMin;
    double yRightMax;

    double biasVRate; //偏置电压速率
    QString title;
    QList<double> points;
    SweepType sweepType;

    bool equal(graphSetup m)
    {
        if(xmin!=m.xmin)
            return false;
        if(xmax!=m.xmax)
            return false;
        if(logX!=m.logX)
            return false;

        if(points!=m.points)
            return false;

        if(sweepType != m.sweepType)
            return false;

        return true;
    }

};

#endif // GLOBLE_H
