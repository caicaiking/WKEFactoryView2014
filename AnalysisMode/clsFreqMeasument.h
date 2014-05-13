#ifndef CLSFREQMEASUMENT_H
#define CLSFREQMEASUMENT_H
#include "clsMeas.h"
#include "Plot.h"
#include <QObject>
#include "clsSettings.h"

class clsFreqMeasument:public clsMeas
{
    Q_OBJECT
public:
    void setMin(double value);
    void setMax(double value);
    void setItems(const QString &item1, const QString &item2);
    void setPoint(QList<double>  *point);
    void setItemsAndPoints(const QString &item1, const QString &item2,
                           QList<double>  *point);
    void trig();
    void stop();
    void setPlot(Plot *plot);
    void setXLog(bool value);
    void setMeter(WKEInstrument *meter);
     void setGraphSettings(graphSetup gs);
signals:
    void showMessage(QString msg);
    void showProgress(int value);
private slots:
    void updatePlot();
private:
    double dblMin;
    double dblMax;
    bool isStop;
    bool xLog;
    Plot *plot;
    QList<double>  *points;
    QString testItem1;
    QString testItem2;

    QVector <double> freq;
    QVector <double> item1;
    QVector <double> item2;

    clsSettings settings;
    WKEInstrument *meter;
};

#endif // CLSFREQMEASUMENT_H
