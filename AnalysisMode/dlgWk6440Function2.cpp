#include "dlgWk6440Function2.h"

dlgWk6440Function2::dlgWk6440Function2(QDialog *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void dlgWk6440Function2::on_btnQ_clicked()
{
    strItem="Q";
    this->accept();
}

void dlgWk6440Function2::on_btnD_clicked()
{
    strItem="D";
    this->accept();
}

void dlgWk6440Function2::on_btnR_clicked()
{
    strItem="R";
    this->accept();
}

void dlgWk6440Function2::on_btnG_clicked()
{
    strItem="G";
    this->accept();
}
