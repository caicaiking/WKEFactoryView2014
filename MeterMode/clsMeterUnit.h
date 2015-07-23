#ifndef CLSMETERUNIT_H
#define CLSMETERUNIT_H

#include "ui_clsMeterUnit.h"

class clsMeterUnit : public QDialog, private Ui::clsMeterUnit
{
    Q_OBJECT

public:
    explicit clsMeterUnit(QWidget *parent = 0);
    void setItem(QString item);
    QString getSuffix();
    void setOFFEnable(bool value);
private slots:
    void on_btnm_clicked();
    void on_btnG_clicked();
    void on_btnM_clicked();
    void on_btnk_clicked();
    void on_btn1_clicked();
    void on_btnu_clicked();
    void on_btnn_clicked();
    void on_btnp_clicked();



    void on_btnNA_clicked();

private:
    QString strSuffix;
};

#endif // CLSMETERUNIT_H
