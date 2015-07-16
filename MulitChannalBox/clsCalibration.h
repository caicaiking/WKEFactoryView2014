#ifndef CLSCALIBRATION_H
#define CLSCALIBRATION_H

#include "ui_clsCalibration.h"
#include "clsDataProcess.h"
#include "clsRuningSettings.h"
class clsCalibration : public QDialog, private Ui::clsCalibration
{
    Q_OBJECT

public:
    explicit clsCalibration(QWidget *parent = 0);
private slots:
    void on_btnCalibration_clicked();
};

#endif // CLSCALIBRATION_H
