#include "dlg4300FunctionMinor.h"

dlg4300FunctionMinor::dlg4300FunctionMinor(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void dlg4300FunctionMinor::on_btnC_clicked()
{
    strItem="C";
    this->accept();
}

void dlg4300FunctionMinor::on_btnD_clicked()
{
    strItem="D";
    this->accept();
}

void dlg4300FunctionMinor::on_btnL_clicked()
{
    strItem="L";
    this->accept();
}

void dlg4300FunctionMinor::on_btnQ_clicked()
{
    strItem="Q";
    this->accept();
}

void dlg4300FunctionMinor::on_btnR_clicked()
{
    strItem="R";
    this->accept();
}

void dlg4300FunctionMinor::on_btnX_clicked()
{
    strItem="X";
    this->accept();
}

void dlg4300FunctionMinor::on_btnG_clicked()
{
    strItem="G";
    this->accept();
}

void dlg4300FunctionMinor::on_btnB_clicked()
{
    strItem="B";
    this->accept();
}

void dlg4300FunctionMinor::on_btnZ_clicked()
{
    strItem="Z";
    this->accept();
}

void dlg4300FunctionMinor::on_btnA_clicked()
{
    strItem=QString("A");
    this->accept();
}

void dlg4300FunctionMinor::on_btnY_clicked()
{
    strItem="Y";
    this->accept();
}

void dlg4300FunctionMinor::on_btnOFF_clicked()
{
    strItem="OFF";
    this->accept();
}
