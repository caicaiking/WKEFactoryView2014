#include "wk6500Range.h"

wk6500Range::wk6500Range(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void wk6500Range::on_btn0_clicked()
{
    Range =tr("自动");
    this->accept();
}

void wk6500Range::on_btn1_clicked()
{
    Range ="1";
    this->accept();
}

void wk6500Range::on_btn2_clicked()
{
    Range ="2";
    this->accept();
}

void wk6500Range::on_btn3_clicked()
{
    Range ="3";
    this->accept();
}

void wk6500Range::on_btn4_clicked()
{
    Range ="4";
    this->accept();
}

void wk6500Range::on_btn5_clicked()
{
    Range ="5";
    this->accept();
}

void wk6500Range::on_btn6_clicked()
{
    Range ="6";
    this->accept();
}

void wk6500Range::on_btn7_clicked()
{
    Range ="7";
    this->accept();
}
