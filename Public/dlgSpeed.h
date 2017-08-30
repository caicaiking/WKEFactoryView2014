#ifndef dlgSPEED_H
#define dlgSPEED_H

#include "ui_dlgSpeed.h"

class dlgSpeed : public QDialog, private Ui::dlgSpeed
{
    Q_OBJECT
    
public:
    explicit dlgSpeed(QWidget *parent = 0);

    QString getSpeed(){return Speed;}

private slots:
    void on_btnMax_clicked();
    void on_btnFast_clicked();
    void on_btnMed_clicked();
    void on_btnSlow_clicked();

    void on_btnCustomer_clicked();

private:
    QString Speed;

};

#endif // dlgSPEED_H
