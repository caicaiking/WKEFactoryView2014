#ifndef WK6500SPEED_H
#define WK6500SPEED_H

#include "ui_wk6500Speed.h"

class wk6500Speed : public QDialog, private Ui::wk6500Speed
{
    Q_OBJECT
    
public:
    explicit wk6500Speed(QWidget *parent = 0);

    QString getSpeed(){return Speed;}

private slots:
    void on_btnMax_clicked();
    void on_btnFast_clicked();
    void on_btnMed_clicked();
    void on_btnSlow_clicked();

private:
    QString Speed;

};

#endif // WK6500SPEED_H
