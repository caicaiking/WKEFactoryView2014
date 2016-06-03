#ifndef CLSMATERIALSETTINGS_H
#define CLSMATERIALSETTINGS_H

#include "ui_clsMaterialSettings.h"
#include "singleton.h"
class clsMaterialSettings : public QDialog, private Ui::clsMaterialSettings
{
    Q_OBJECT

public:
    explicit clsMaterialSettings(QWidget *parent = 0);
private slots:
    void on_btnClose_clicked();


    void on_grpContact_toggled(bool arg1);

    void on_grpNoContact_toggled(bool arg1);

    void on_btnConD_clicked();

    void on_btnConTm_clicked();

    void on_btnNonConTg_clicked();

    void on_btnNonConTm_clicked();

    void on_btnN_clicked();

    void on_btnL_clicked();

    void on_btnA_clicked();

    void on_btnMeasCg_clicked();

    void on_btnMeasLwRw_clicked();

private:
    double conD, conTm;
    double nonConTg,nonConTm;
    int N;
    double l;
    double A;
    void updateButtons();
    void updateGpibCommands();
    void showProgress(int i);
    bool PopUpmessage(QString strMessage, QString strTitle);
};


typedef Singleton<clsMaterialSettings> sngMaterialSettings;
#endif // CLSMATERIALSETTINGS_H
