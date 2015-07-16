#ifndef CLSCALIBRATION_H
#define CLSCALIBRATION_H

#include "ui_clsCalibration.h"
#include "clsDataProcess.h"
#include "clsRuningSettings.h"
#include "clsConnectSWBox.h"
class clsCalibration : public QDialog, private Ui::clsCalibration
{
    Q_OBJECT

public:
    explicit clsCalibration(QWidget *parent = 0);
private slots:

    void on_btnTest_clicked();
    void on_btnOpen_clicked();
    void on_btnShort_clicked();

private:
    double ocZ,ocA,scZ,scA;
};

#endif // CLSCALIBRATION_H
