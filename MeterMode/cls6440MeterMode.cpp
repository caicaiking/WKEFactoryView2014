#include "cls6440MeterMode.h"
#include <QDebug>
cls6440MeterMode::cls6440MeterMode(QWidget *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);
}

void cls6440MeterMode::setCondition(const QString &value)
{
    this->lineEdit->setText(value);
}

QString cls6440MeterMode::getTestItem()
{

}

void cls6440MeterMode::trig()
{
    emit testResult("L,512.03nH,PASS,C,-2.003nF,FAIL,R,1.2354mΩ,PASS,C,-2.003nF,FAIL",4);
}

void cls6440MeterMode::start()
{
    emit startTest("1111,11111,11111");
    qDebug()<<"meter test!";
}

QString cls6440MeterMode::getTestCondition()
{
    return lineEdit->text();
}

QString cls6440MeterMode::getBrief()
{
    return "[LCR] 100kHz@1V Series L R RDC"+lineEdit->text();
}

FunctionType cls6440MeterMode::getType()
{
    return MeterFunction;
}
