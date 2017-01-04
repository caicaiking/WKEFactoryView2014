#ifndef CLSMEAS_H
#define CLSMEAS_H

#include <QObject>

#include "Plot.h"
#include "frmTraceSetup.h"
#include "WKEInstrument.h"
#include <QList>

class clsMeas:public QObject
{
    Q_OBJECT
public:
    virtual void setMin(double)=0;
    virtual void setMax(double)=0;
    virtual void setXLog(bool value)=0;
    virtual void setItems(const QString &item1, const QString &item2)=0;
    virtual void setPoint(QList<double> *point)=0;
    virtual void setItemsAndPoints(const QString &item1, const QString &item2,
                                   QList<double>  *point)=0;
    virtual void trig()=0;
    virtual void stop()=0;
    virtual void setPlot(Plot *plot)=0;
    virtual void setMeter(WKEInstrument *meter)=0;
    virtual void setGraphSettings(graphSetup gs)=0;
signals:
    void showMessage(QString msg);
    void showProgress(int value);
    void showTestValue(double value);
};


#endif // CLSMEAS_H
