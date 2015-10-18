#include "cls3260minorfunction.h"

cls3260MinorFunction::cls3260MinorFunction(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

}

QString cls3260MinorFunction::getItem() const
{
    return this->item;
}

void cls3260MinorFunction::on_btnQ_clicked()
{
    item="Q";
    this->accept();
}

void cls3260MinorFunction::on_btnD_clicked()
{
    item="D";
    this->accept();
}

void cls3260MinorFunction::on_btnR_clicked()
{
    item="R";
    this->accept();
}
