#include "PlotSettings.h"
#include <QList>
#include <cmath>
PlotSettings::PlotSettings()
{
    minX= 0.0;
    maxX = 10.0;



    minY = 0.0;
    maxY = 10.0;

    adjust();


}

void PlotSettings::adjust()
{
    this->numXTicks= adjustAxis(minX,maxX);
    this->numYTicks = adjustAxis(minY,maxY);
}

QList<double> PlotSettings::adjustAxis(double min, double max)
{
    if(min> max)
        qSwap(min,max);
    int numTicks=0;
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if (5 * step < grossStep) {
        step *= 5;
    } else if (2 * step < grossStep) {
        step *= 2;
    }

    numTicks = int(std::ceil(max / step) - std::floor(min / step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    double minC = std::floor(min / step) * step;
    //double maxC = std::ceil(max / step) * step;

    QList <double> ticks;

    int i = 0;
    while((minC + step * i) <= max)
    {
        if ((minC + step * i)>=min)
            ticks.append(minC+ step *i);
        i++;
    }

    return ticks;
}

