#ifndef PLOT_H
#define PLOT_H
#include "Globle.h"
#include <qwt_plot.h>
#include <QWidget>
#include <clsRefTraceProperty.h>
#include <qwt_plot_curve.h>
#include "clsCurveLimit.h"
#include <qwt_legend.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotCurve;
class QwtPlotGrid;


class Plot: public QwtPlot
{
    Q_OBJECT
public:

    enum Choice{
        yLeft, yRight,Both
    };

    Plot(QWidget *parent=0);
    void showData(const QVector<double> &x,
                  const QVector<double>& curvel,
                  const QVector<double> & curve2);
    void autoScale(Choice choice);
    void setGraphTitle(const QString &);
    void setTraceA(const QString & value);
    void setTraceB(const QString & value);
    void setXTrace(const QString & value);
    QString getTraceA(){return strTraceA;}
    QString getTraceB(){return strTraceB;}
    QString getXTrace(){return strXTrace;}
    void setTraces(const QString &traceA="Z",
                   const QString &traceB="θ",
                   const QString &xTrace=tr("频率[Hz]"));

    void setXScale(double min, double max, bool logx);
    void setYLeftScal(double min, double max, bool logx);
    void setYRightScal(double min, double max);
    void setScale(double xmin=100,double xmax=100000,bool blLogX=true,
                  double yLeftMin=100,double yLeftMax=100000,bool blLogYLeft=true,
                  double yRightMin=-100, double yRightMax=100);
  void getScale(double &xmin,  double &xmax, bool &blLogX,
                 double &yLeftMin, double &yLeftMax, bool &blLogYLeft,
                 double &yRightMin, double &yRightMax);
    void turnOffCurves(Choice choice, bool status=true);
    QList<PlotCurves> getCurves(){return this->curves.values();}
    void turnOffCurves( QList<curveProperty> cp, Plot::Choice choice, bool status);
    double getXmax() const;
    double getXmin() const;
    double getYLeftMax() const;
    double getYLeftMin() const;
    double getYRightMax() const;
    double getYRightMin() const;
    bool getBlLogX() const;
    void setBlLogX(bool value);
    bool getBlLogYLeft() const;
    void setBlLogYLeft(bool value);
    bool getYLeftIsEnable();
    bool getYRightIsEnable();

    void clearData();
    void setToTop(curveProperty property, bool isOnTop);
    void turnOffRefTrace();

    void findPeak(Choice x, bool peakType);
    void findNextLeftHigh(Choice x);
    void findNextLeftLow(Choice x);
    void findNextRightHigh(Choice x);
    void findNextRightLow(Choice x);
    QMap<double, double> fff(QMap<double,double> list, double tmpRet);
    double getCurrentMarkValue();
    QMap<double, double> getChoiceData(Choice x);

    void setCurveLimit(const clsCurveLimit &curveLimit);
    void setCurveLimitVisiable(const bool &value);

    void setMultMarker(bool value);
    void setCurrentMarker(int value);
    void setMarkerVisual(int i, bool value);
    void setDataEnd();
    void setShowPercetage(bool value);

signals:
    void showMarkerMessage(int , QString);

public slots:
    QString setMarker(const double& freq, const int intSelected=0);
    void addNewCurve(curveProperty property, bool isSetCurrent=false);
    void addNewCurve(curveProperty property, QVector<double> x, QVector<double> y1, QVector<double> y2);
protected:
    void mouseMoveEvent(QMouseEvent *e);

private:

    QString strTraceA , strTraceB, strXTrace;

    //为了可以显示多条曲线
    QMap <int,PlotCurves> curves;

    QwtPlotMarker *d_marker1,*traceAUpLimit,*traceADownLimit,*traceBUpLimit,*traceBDownLimit;
    bool multiMarker;
    int currentMarker;

    QList<QwtPlotMarker *> markers;

    QwtPlotGrid *gridX;
    QwtPlotGrid *gridYLeft;
    QwtPlotGrid *gridYRight;


    bool blLogX;
    bool blLogYLeft;
    double dblMarkValue;

    double xmax;
    double xmin;
    double yLeftMax;
    double yLeftMin;
    double yRightMax;
    double yRightMin;

    clsCurveLimit curveLimit;

    bool showPercetage;
};

#endif // PLOT_H
