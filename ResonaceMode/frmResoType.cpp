#include "frmResoType.h"

frmResoType::frmResoType(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

QString frmResoType::getResonanceType()
{
    return this->res;
}

void frmResoType::on_btnSeries_clicked()
{
    this->res= btnSeries->text();
    this->accept();
}

void frmResoType::on_btnParallel_clicked()
{
    this->res=btnParallel->text();
    this->accept();
}

void frmResoType::on_btnCrystal_clicked()
{
    this->res=this->btnCrystal->text();
    this->accept();
}
