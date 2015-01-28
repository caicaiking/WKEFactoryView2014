#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include "ui_frmSettings.h"

class frmSettings : public QDialog, private Ui::frmSettings
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent = 0);
};

#endif // FRMSETTINGS_H
