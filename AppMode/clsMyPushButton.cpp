#include "clsMyPushButton.h"

clsMyPushButton::clsMyPushButton(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void clsMyPushButton::setText(QString value)
{
    this->btnApp->setText(value);
}

void clsMyPushButton::on_btnApp_clicked()
{
    emit btnClick(this->btnApp->text());
}
