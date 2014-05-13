#ifndef WK6500CALIBRATION_H
#define WK6500CALIBRATION_H

#include "ui_wk6500Calibration.h"


class wk6500Calibration : public QDialog, private Ui::wk6500Calibration
{
    Q_OBJECT
    
public:
    explicit wk6500Calibration(double maxFreq,QWidget *parent = 0);

    static QString GetModel();
    static double GetMaxFrequency();

    void readSettings();
    void writeSettings();
    void sleepMs(int svalue);
    void Increase();
    void PopUpmessage(QString strMessage, QString strTitle);
    void setMaxFreq(double value);
private slots:
    void on_btnOpen_clicked();

    void on_btnSetLimit_clicked();

    void on_btnDone_clicked();


    void on_btnClearOpenShort_clicked();

    void on_btnShort_clicked();

    void on_btnHF_clicked();

    void on_btnClearHF_clicked();

private:
    double dblMaxFrequency;
    QString strMode;

    double dblCalibrateFrequecy;
};

#endif // WK6500CALIBRATION_H
