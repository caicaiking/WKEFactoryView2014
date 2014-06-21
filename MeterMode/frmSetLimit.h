#ifndef FRMSETLIMIT_H
#define FRMSETLIMIT_H

#include "ui_frmSetLimit.h"
#include "clsMeterLimit.h"

class frmSetLimit : public QDialog, private Ui::frmSetLimit
{
    Q_OBJECT

public:
    explicit frmSetLimit(QWidget *parent = 0);

    clsMeterLimit getMeterLimit();

    void setLimits(clsMeterLimit value);
    void setItem(QString value);
private slots:
    void on_rbAbs_toggled(bool checked);
    void on_rbPer_toggled(bool checked);
    void updateButtons();
    void on_btnW_clicked();
    void on_btnN_clicked();
    void on_btnM_clicked();
    void on_btnK_clicked();
    void on_btnJ_clicked();
    void on_btnG_clicked();
    void on_btnF_clicked();
    void on_btnD_clicked();
    void on_btnC_clicked();
    void on_btnB_clicked();
    void on_btnAbsHi_clicked();
    void on_btnAbsLow_clicked();
    void on_btnPerNorminal_clicked();
    void on_btnPerHi_clicked();
    void on_btnPerLo_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    clsMeterLimit m1;
    QString item;
};

#endif // FRMSETLIMIT_H
