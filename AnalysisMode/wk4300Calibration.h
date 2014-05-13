#ifndef WK4300CALIBRATION_H
#define WK4300CALIBRATION_H

#include "ui_wk4300Calibration.h"

class wk4300Calibration : public QDialog, private Ui::wk4300Calibration
{
    Q_OBJECT

    void PopUpmessage(QString strMessage, QString strTitle);
    void readSettings();
    void writeSettings();
public:
    explicit wk4300Calibration(QWidget *parent = 0);
    void showProgress(int i);
private slots:
    void on_btnOpen_clicked();

    void on_btnOk_clicked();
    void on_btnShort_clicked();
};

#endif // WK4300CALIBRATION_H
