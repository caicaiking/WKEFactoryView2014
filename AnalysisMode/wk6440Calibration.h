#ifndef WK6440CALIBRATION_H
#define WK6440CALIBRATION_H

#include "ui_wk6440Calibration.h"

class wk6440Calibration : public QDialog, private Ui::wk6440Calibration
{
    Q_OBJECT


public:
    explicit wk6440Calibration(QWidget *parent = 0);

private slots:
    void on_btnDone_clicked();

    void on_btnOpen_clicked();

    void on_btnShort_clicked();

    void on_btnHF_clicked();

private:
    void readSettings();
    void writeSettings();
    void Increase();
    void sleepMs(int svalue);
    void PopUpmessage(QString strMessage, QString strTitle);
    bool getCalibrationRes();
    void DelaySec(int value);
};

#endif // WK6440CALIBRATION_H
