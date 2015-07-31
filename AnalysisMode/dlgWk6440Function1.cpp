#include "dlgWk6440Function1.h"
#include"QDebug"

dlgWk6440Function1::dlgWk6440Function1(QDialog *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void dlgWk6440Function1::on_btnC_clicked()
{
    strItem="C";
    this->accept();
}

void dlgWk6440Function1::on_btnL_clicked()
{
    strItem="L";
    this->accept();
}

void dlgWk6440Function1::on_btnB_clicked()
{

    strItem ="B";
    this->accept();

}

void dlgWk6440Function1::on_btnZ_clicked()
{
    strItem="Z";
    this->accept();
}

void dlgWk6440Function1::on_btnY_clicked()
{
    strItem="Y";
    this->accept();
}

void dlgWk6440Function1::on_btnX_clicked()
{

    strItem ="X";
    this->accept();


}
