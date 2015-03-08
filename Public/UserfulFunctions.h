#ifndef USERFULFUNCTIONS_H
#define USERFULFUNCTIONS_H
#include <QMap>
#include "Plot.h"
#include <Qwt/qwt_plot_curve.h>
#include <QList>
#include "Globle.h"
#include "Plot.h"

#include <QString>
#include <QStringList>

class UserfulFunctions
{
public:
    UserfulFunctions();

    static bool IsDouble(const QStringList r);
    static bool IsDouble(const QString r);

    static QString getSuffix(const QString& value);
    static QString getName(const QString &value);
    static void sleepMs(int svalue);
    static QMap<double, QPointF> getPlotCurveData(const  QList<PlotCurves> &value,
                                                  int intSelect=0);
    static double Max(const QList<double> &arry);
    static double Min(const QList<double> &arry);
    static double getNearItem(const QList<double> &arry, const double value, const bool isLog=false);
    static double Abs(double x);
    static QList<QPointF> getPlotCurveData(QwtPlotCurve *curve);
    static QList<double> resultPro(const QString &resultString);


    static QString getSweepTypeName(SweepType t);

   static QString getSweepTypeUnit(SweepType t);

 static const  QString getSweepFunctionSuffix(SweepType value);
};

#endif // USERFULFUNCTIONS_H
