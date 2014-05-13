#ifndef CLSTIMEMEASUMENT_H
#define CLSTIMEMEASUMENT_H
#include "clsMeas.h"
#include <QObject>
#include <QTimer>
#include <QVector>
class clsTimeMeasument : public clsMeas
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
    void singleShot();
private:
    double dblMin;
    double dblMax;
    bool isStop;
    QVector <double> times;
    QVector <double> item1;
    QVector <double> item2;
    Plot *plot;
    QList<double>  *points;
    QString testItem1;
    QString testItem2;
    QTime startTime;


    QTimer timer;
    void setTimer();
WKEInstrument *meter;
    double average(const QVector<double> &tmp);
};

#endif // CLSTIMEMEASUMENT_H
