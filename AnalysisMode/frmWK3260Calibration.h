#ifndef FRMWK3260CALIBRATION_H
#define FRMWK3260CALIBRATION_H

#include "ui_frmWK3260Calibration.h"

class frmWK3260Calibration : public QDialog, private Ui::frmWK3260Calibration
{
    Q_OBJECT

public:
    explicit frmWK3260Calibration(QWidget *parent = 0);
};

#endif // FRMWK3260CALIBRATION_H
