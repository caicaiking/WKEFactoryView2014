#ifndef CLSMULTILIMITSETTING_H
#define CLSMULTILIMITSETTING_H

#include "ui_clsMultiLimitSetting.h"
#include "clsMeterLimit.h"
#include "clsMultiLimits.h"
class clsMultiLimitSetting : public QDialog, private Ui::clsMultiLimitSetting
{
    Q_OBJECT

public:
    explicit clsMultiLimitSetting(QWidget *parent = 0);
    void setConditon(const QString &item1, const QString &item2, const clsMultiLimits &limits);
clsMultiLimits getLimts();
private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

    void lblItem1Click();
    void lblItem2Click();
    void lblFrequencClick();





    void on_grpTraceA_toggled(bool arg1);

    void on_grpTraceB_toggled(bool arg1);

private:
    QString item1;
    QString item2;
    clsMultiLimits limits;
    void updateLabels();
};

#endif // CLSMULTILIMITSETTING_H
