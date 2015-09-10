#include "clsMarkerItem.h"

clsMarkerItem::clsMarkerItem(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    isActive = false;
}

void clsMarkerItem::setNumber(int number)
{
    this->number = number;
    btnNumber->setText(QString::number(number+1));
}

void clsMarkerItem::on_btnNumber_clicked()
{
    emit clicked(this->number);
    // btnNumber->setStyleSheet("color: red");
}

void clsMarkerItem::setActive(int value)
{
    if (value == number)
        btnNumber->setStyleSheet("color: red");
    else
        btnNumber->setStyleSheet("color: black");

    isActive = (value == number);
}

void clsMarkerItem::setText(QString text)
{
    this->lblText->setText(text);

    lblText->setVisible(!text.isEmpty());


}
