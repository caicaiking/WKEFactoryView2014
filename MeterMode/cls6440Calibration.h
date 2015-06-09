#ifndef CLS6440CALIBRATION_H
#define CLS6440CALIBRATION_H

#include "ui_cls6440Calibration.h"

class cls6440Calibration : public QDialog, private Ui::cls6440Calibration
{
    Q_OBJECT

public:
    explicit cls6440Calibration(QWidget *parent = 0);
};

#endif // CLS6440CALIBRATION_H
