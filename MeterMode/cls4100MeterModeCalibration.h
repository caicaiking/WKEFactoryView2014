#ifndef CLS4100METERMODECALIBRATION_H
#define CLS4100METERMODECALIBRATION_H

#include "ui_cls4100MeterModeCalibration.h"

class cls4100MeterModeCalibration : public QDialog, private Ui::cls4100MeterModeCalibration
{
    Q_OBJECT


public:
    explicit cls4100MeterModeCalibration(QWidget *parent = 0);
private slots:
    void on_btnOpenSpot_clicked();
    void on_btnOpen10kHz_clicked();
    void on_btnOpen100kHz_clicked();
    void on_btnOpenAllFreq_clicked();
    void on_btnShortSpot_clicked();
    void on_btnShort10kHz_clicked();
    void on_btnShort100kHZ_clicked();
    void on_btnShortAllFreq_clicked();
    void on_btnClose_clicked();
private:
    void readSettings();
    void saveSettings();
    bool PopUpmessage(QString strMessage, QString strTitle);
    void Increase();
    void sleepMs(int svalue);
    bool getCalibrationRes();
    void DelaySec(int value);
    QString openTrim(int type, int delay);
    QString shortTrim(int type, int delay);
};

#endif // CLS4100METERMODECALIBRATION_H
