#include "clsFit.h"

clsFit::clsFit(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void clsFit::setTraceTitle(QString strTraceA, QString strTraceB)
{
    this->btnTraceA->setText(strTraceA);
    this->btnTraceB->setText(strTraceB);
}

void clsFit::on_btnBoth_clicked()
{
    choice=Plot::Both;
    this->accept();
}

void clsFit::on_btnTraceA_clicked()
{
    choice=Plot::yLeft;
    this->accept();
}

void clsFit::on_btnTraceB_clicked()
{
    choice=Plot::yRight;
    this->accept();
}
