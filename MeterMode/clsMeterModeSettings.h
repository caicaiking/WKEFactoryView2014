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
    bool getSingleRes() const;
    void setSingleRes(bool value);

    int getStartNumber() const;
    void setStartNumber(int value);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    int startNumber;
    bool singleRes;
};

#endif // CLSMETERMODESETTINGS_H
