#include "cls3260majorfunction.h"

cls3260MajorFunction::cls3260MajorFunction(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

}

QString cls3260MajorFunction::getItem() const
{
    return item;
}

void cls3260MajorFunction::on_btnL_clicked()
{
    item="L";
    this->accept();
}

void cls3260MajorFunction::on_btnC_clicked()
{
    item ="C";
    this->accept();
}

void cls3260MajorFunction::on_btnZ_clicked()
{
    item="Z";
    this->accept();
}
