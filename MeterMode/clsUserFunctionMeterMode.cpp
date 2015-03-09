#include "clsUserFunctionMeterMode.h"
#include <QDebug>
clsUserFunctionMeterMode::clsUserFunctionMeterMode(QWidget *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);
}

//设置测试条件
void clsUserFunctionMeterMode::setCondition(const QString &value)
{
    this->lineEdit->setText(value);
}

QString clsUserFunctionMeterMode::getTestItem()
{
    //不会产生测试数据，所以返回值为空
    return "";
}

QString clsUserFunctionMeterMode::trig()
{
     emit testResult("love cc",0);
    return "";
}

void clsUserFunctionMeterMode::start()
{
    qDebug()<<"Userful function test";
}

QString clsUserFunctionMeterMode::getTestCondition()
{
    return this->lineEdit->text();
}

QString clsUserFunctionMeterMode::getBrief()
{
    return "[UF]Delay "+lineEdit->text();
}

FunctionType clsUserFunctionMeterMode::getType()
{
    return UF;
}

bool clsUserFunctionMeterMode::getPassFail()
{

}

void clsUserFunctionMeterMode::turnOffBias()
{

}

void clsUserFunctionMeterMode::on_pushButton_clicked()
{
    emit testResult("love cc",0);
}
