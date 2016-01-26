#ifndef CLSMETERMODESETTINGS_H
#define CLSMETERMODESETTINGS_H

#include "ui_clsMeterModeSettings.h"
#include "UserfulFunctions.h"
class clsMeterModeSettings : public QDialog, private Ui::clsMeterModeSettings
{
    Q_OBJECT

public:
    explicit clsMeterModeSettings(QWidget *parent = 0);
    void setCondition(const MeterSettings &st);

    MeterSettings getCondtion();


private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
};

#endif // CLSMETERMODESETTINGS_H
