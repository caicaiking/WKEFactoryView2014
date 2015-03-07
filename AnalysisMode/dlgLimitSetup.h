#ifndef DLGLIMITSETUP_H
#define DLGLIMITSETUP_H

#include "ui_dlgLimitSetup.h"
#include "clsMeterLimit.h"
#include "frmSetLimit.h"
#include "WKEInstrument.h"
#include <QColor>
#include "clsCurveLimit.h"
class dlgLimitSetup : public QDialog, private Ui::dlgLimitSetup
{
    Q_OBJECT

public:
    explicit dlgLimitSetup(WKEInstrument*meter,QWidget *parent = 0);

    void setCurveLimit(const clsCurveLimit & curveLimit);
    clsCurveLimit getCurveLimit();

private slots:
    void traceAClick();
    void traceBClick();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

    void setTraceAUpColor();
    void setTraceADownColor();
    void setTraceBUpColor();
    void setTraceBDownColor();
    void on_btnAddLimit_clicked();

private:
    clsMeterLimit traceALimit;
    clsMeterLimit traceBLimit;

    QColor traceAUpColor;
    QColor traceBUpColor;
    QColor traceADownColor;
    QColor traceBDownColor;
    WKEInstrument* meter;

    void updateButtons();
    QString getStyleSheet(const QColor &color);
};

#endif // DLGLIMITSETUP_H
