#ifndef USERFULFUNCTIONS_H
#define USERFULFUNCTIONS_H
#include <QMap>
#include <Plot.h>
class QStringList;
class QString;
#include "Globle.h"
class UserfulFunctions
{
public:
    UserfulFunctions();

    static bool IsDouble(const QStringList r);
    static bool IsDouble(const QString r);

    static QString getSuffix(const QString& value);
    static QString getName(const QString &value);
    static void sleepMs(int svalue);
    static QMap<double, QPointF> formatToData(const  QList<PlotCurves> &value, int intSelect=0);
    static double Max(const QList<double> &arry);
    static double Min(const QList<double> &arry);
    static double NearItem(const QList<double> &arry, const double value, const bool isLog=false);
    static double Abs(double x);
    static QList<QPointF> formatToData(QwtPlotCurve *curve);
    static QList<double> resultPro(const QString &resultString);


    static QString getSweepTypeName(SweepType t)
    {
        switch (t) {
        case Time:
            return QObject::tr("时间");
            break;
        case frequency:
            return QObject::tr("频率");
            break;
        case biasV:
            return QObject::tr("偏置电压");
            break;
        case biasA:
            return QObject::tr("偏置电流");
            break;
        case levelV:
            return QObject::tr("电压信号");
            break;
        case levelA:
            return QObject::tr("电流信号");
            break;
        default:
            break;
        }
        return "";
    }

   static QString getSweepTypeUnit(SweepType t)
    {
        switch (t) {
        case Time:
            return "s";
            break;
        case frequency:
            return "Hz";
            break;
        case biasV:
            return "V";
            break;
        case biasA:
            return "A";
            break;
        case levelV:
            return "V";
            break;
        case levelA:
            return "A";
            break;
        default:
            break;
        }

        return "";
    }

};

#endif // USERFULFUNCTIONS_H
