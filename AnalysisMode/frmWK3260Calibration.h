#ifndef FRMWK3260CALIBRATION_H
#define FRMWK3260CALIBRATION_H

#include "ui_frmWK3260Calibration.h"

class frmWK3260Calibration : public QDialog, private Ui::frmWK3260Calibration
{
    Q_OBJECT

    void PopUpmessage(QString strMessage, QString strTitle);
    void writeSettings();
    void readSettings();
    int getCalRes();
    void showProgress(int delayTime);
    void switchToImpMode();
    void switchToCalMode();
    void setCalNorm();
    void setCalBoost();
    QString showInformation(QString calType, int calRes);
    QString getGpibMeter();
public:
    explicit frmWK3260Calibration(QWidget *parent = 0);
private slots:
    void on_btnOk_clicked();
    void on_btnOpenNorm_clicked();
    void on_btnShortNorm_clicked();
    void on_btnHFNorm_clicked();
    void on_btnOpenBoost_clicked();
    void on_btnShortBoost_clicked();
    void on_btnHFBoost_clicked();
};

#endif // FRMWK3260CALIBRATION_H
