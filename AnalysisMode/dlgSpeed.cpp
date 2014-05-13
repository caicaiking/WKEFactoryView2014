#include "dlgSpeed.h"

dlgSpeed::dlgSpeed(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void dlgSpeed::on_btnMax_clicked()
{
    Speed =tr("最快");
    this->accept();
}

void dlgSpeed::on_btnFast_clicked()
{
     Speed =tr("快速");
    this->accept();
}

void dlgSpeed::on_btnMed_clicked()
{
     Speed =tr("中速");
    this->accept();
}

void dlgSpeed::on_btnSlow_clicked()
{
     Speed =tr("慢速");
    this->accept();
}
