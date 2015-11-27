#ifndef PLOTDATAANLYSIS_H
#define PLOTDATAANLYSIS_H

#include "PlotSettings.h"
#include <QVector>
#include <QPointF>
struct SINGLE_BAR{
    int barValue;
    double x;
    double y;
};

class PlotDataAnlysis
{
public:
    PlotDataAnlysis();

    void setTestData(const QVector<double> &testData);

    PlotSettings getSettings() const;
    QVector<SINGLE_BAR> getBars();
    QVector<QPointF> getFitData();

    double getMeanValue() const;
    double getStdValue() const;
    double getNeg3Std() const;
    double getPos3Std() const;

private:
    QVector<SINGLE_BAR> getBarStatic();
    QVector<QPointF> getNormCurve();

    QVector<double> testData;
    PlotSettings pltSettings;
    QVector<SINGLE_BAR> bars;
    QVector<QPointF> fitData;

    double meanValue;
    double stdValue;
    double neg3Std;
    double pos3Std;

};

#endif // PLOTDATAANLYSIS_H
