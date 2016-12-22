#ifndef CLSMULTICHANNELSETTINGS_H
#define CLSMULTICHANNELSETTINGS_H

#include "ui_clsMultiChannelSettings.h"

class clsMultiChannelSettings : public QDialog, private Ui::clsMultiChannelSettings
{
    Q_OBJECT

public:
    explicit clsMultiChannelSettings(QWidget *parent = 0);
    void setCondition(int delay, bool isUseLoadData);
    int getSwitchDelay();
    bool isUseLoadData();
    bool getTurnOnScreen();
    void setCondition(bool isFastGpibOn);
    void setLedLightEnable(bool value);
    bool getLedEnable();
private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
};

#endif // CLSMULTICHANNELSETTINGS_H
