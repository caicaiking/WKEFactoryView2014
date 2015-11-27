#ifndef PLOTSETTINGS_H
#define PLOTSETTINGS_H
#include <QList>

class PlotSettings
{
public:
    PlotSettings();

    double minX;
    double maxX;
    QList<double> numXTicks;
    double minY;
    double maxY;
    QList<double> numYTicks;

    double spanX() const { return maxX- minX;}
    double spanY() const { return maxY - minY;}

    void adjust();

    static QList<double> adjustAxis(double min, double max);

};

#endif // PLOTSETTINGS_H
