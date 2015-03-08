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

    void setCurveLimit(const clsCurveLimit &value);
    void setMultiCurveLimit(const clsMultiCurveLimt &value);

    WKEInstrument *getMeter() const;
    void setMeter(WKEInstrument *value);

private slots:

    void on_lblClose_clicked();

private:
    clsCurveLimit curveLimit;
    clsMultiCurveLimt multiCurveLimit;
    WKEInstrument *meter;
};

#endif // CLSMULTILIMITSTATUSSHOW_H
