#ifndef CLS6440CALIBRATION_H
#define CLS6440CALIBRATION_H

#include "ui_cls6440Calibration.h"

class cls6440Calibration : public QDialog, private Ui::cls6440Calibration
{
    Q_OBJECT

    bool PopUpmessage(QString strMessage, QString strTitle);
    void Increase();
    void sleepMs(int svalue);
    bool getCalibrationRes();
    void DelaySec(int value);
    QString openTrim(int type, int delay);
    QString shortTrim(int type, int delay);
public:
    explicit cls6440Calibration(QWidget *parent = 0);
    void readSettings();
    void saveSettings();
private slots:

    void on_btnClose_clicked();
    void on_btnOpenSpot_clicked();
    void on_btnOpen10kHz_clicked();
    void on_btnOpen100kHz_clicked();
    void on_btnOpenAllFreq_clicked();
    void on_btnShortSpot_clicked();
    void on_btnShort10kHz_clicked();
    void on_btnShort100kHZ_clicked();
    void on_btnShortAllFreq_clicked();
    void on_btnHF_clicked();
};

#endif // CLS6440CALIBRATION_H
