#include "dlgWk6440Function1.h"
#include"QDebug"

dlgWk6440Function1::dlgWk6440Function1(QDialog *parent) :
    QDialog(parent)
{
    setupUi(this);
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
    QString btn;
    WKEInstrument * myParent = static_cast<WKEInstrument *>(this->parent());

    if(myParent == 0)
        return;
    btn = myParent->getEqucct();

    if(btn==tr("并联"))
    {
        strItem="B";
        this->accept();
    }
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
    QString btn;
    WKEInstrument * myParent = static_cast<WKEInstrument *>(this->parent());

    if(myParent == 0)
        return;

    btn = myParent->getEqucct();

    if(btn==tr("串联"))
    {
        strItem="X";
        this->accept();
    }
}
