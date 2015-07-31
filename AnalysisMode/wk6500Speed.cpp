#include "wk6500Speed.h"

wk6500Speed::wk6500Speed(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void wk6500Speed::on_btnMax_clicked()
{
    Speed ="MAX";
    this->accept();
}

void wk6500Speed::on_btnFast_clicked()
{
    Speed ="FAST";
    this->accept();
}

void wk6500Speed::on_btnMed_clicked()
{
    Speed ="MED";
    this->accept();
}

void wk6500Speed::on_btnSlow_clicked()
{
    Speed ="SLOW";
    this->accept();
}
