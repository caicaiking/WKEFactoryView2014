#ifndef CLSMULTILIMITSTATUSSHOW_H
#define CLSMULTILIMITSTATUSSHOW_H

#include "ui_clsMultiLimitStatusShow.h"
#include "clsMultiCurveLimt.h"
#include "clsCurveLimit.h"
#include "WKEInstrument.h"
class clsMultiLimitStatusShow : public QDialog, private Ui::clsMultiLimitStatusShow
{
    Q_OBJECT

public:
    explicit clsMultiLimitStatusShow(QWidget *parent = 0);
    void updateTable();

    void setCurveLimit(clsCurveLimit value);
    void setMultiCurveLimit( clsMultiCurveLimt value);

    void setInfo(QString value);

    void setMeter(WKEInstrument *value);

public slots:
    void updateStatus();

private slots:



private:
    clsCurveLimit curveLimit;
    clsMultiCurveLimt multiCurveLimit;
    WKEInstrument *meter;
    void setLabel(double freq, double item1, double item2, bool status);
    QLabel *getStatusLabel(const bool &value=true);
};

#endif // CLSMULTILIMITSTATUSSHOW_H
