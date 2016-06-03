#ifndef FRMTRACESETUP_H
#define FRMTRACESETUP_H

#include "ui_frmTraceSetup.h"
#include <QObject>
#include "Globle.h"
#include "clsInputLimitCheck.h"
#include  "WKEInstrument.h"
class frmTraceSetup : public QDialog, private Ui::frmTraceSetup
{
    Q_OBJECT

public:
    explicit frmTraceSetup(WKEInstrument* meter,QWidget *parent = 0);
    static bool writeSettings(graphSetup gsetup);
    static bool readSettings(graphSetup &gsetup, bool incldSwty = true);

    void setMeasSettings(WKEInstrument* meter)
    {
        this->meter= meter;

    }
    graphSetup getGsetup() const;

private slots:
    void on_btnTime_clicked();
    void updateSweepTitile(SweepType t);
    void on_btnFrequency_clicked();
    void on_btnBias_clicked();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnXmin_clicked();
    void on_chkXlog_clicked();
    void on_chkYleftLog_clicked();
    void on_btnXmax_clicked();
    void on_btnYLeftMin_clicked();
    void on_btnYRightMin_clicked();
    void on_btnYLeftMax_clicked();
    void on_btnYRightMax_clicked();
    void on_btnTitle_clicked();
    void on_btnPoints_clicked();

    void on_btnBiasA_clicked();

    void on_btnRate_clicked();

    void on_btnBiasExt_clicked();

private:
    graphSetup gsetup;
    LimitCheck *checkLimit;
    void updateButtons();
    WKEInstrument* meter;

    void getSupportFunction();
};

#endif // FRMTRACESETUP_H
