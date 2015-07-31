#include "dlg4300FuctionMajor.h"

dlg4300FuctionMajor::dlg4300FuctionMajor(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void dlg4300FuctionMajor::on_btnC_clicked()
{
    strItem="C";
    this->accept();
}

void dlg4300FuctionMajor::on_btnD_clicked()
{
    strItem="D";
    this->accept();
}

void dlg4300FuctionMajor::on_btnL_clicked()
{
    strItem="L";
    this->accept();
}

void dlg4300FuctionMajor::on_btnQ_clicked()
{
    strItem="Q";
    this->accept();
}

void dlg4300FuctionMajor::on_btnR_clicked()
{
    strItem="R";
    this->accept();
}

void dlg4300FuctionMajor::on_btnX_clicked()
{
    strItem="X";
    this->accept();
}

void dlg4300FuctionMajor::on_btnG_clicked()
{
    strItem="G";
    this->accept();
}

void dlg4300FuctionMajor::on_btnB_clicked()
{
    strItem="B";
    this->accept();
}

void dlg4300FuctionMajor::on_btnZ_clicked()
{
    strItem="Z";
    this->accept();
}

void dlg4300FuctionMajor::on_btnA_clicked()
{
    strItem=QString("A");
    this->accept();
}

void dlg4300FuctionMajor::on_btnY_clicked()
{
    strItem="Y";
    this->accept();
}

void dlg4300FuctionMajor::on_btnRdc_clicked()
{
    strItem="Rdc";
    this->accept();
}


//void dlgFunction::on_btnOFF_clicked()
//{
//    strItem="OFF";
//    this->accept();
//}
