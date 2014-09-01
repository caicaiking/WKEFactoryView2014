#include "PassFailLabel.h"

PassFailLabel::PassFailLabel(QWidget *parent) :
    QLabel(parent)
{
}

void PassFailLabel::showText(const QString &value)
{
    this->setText(value);

    if(value.toUpper() =="PASS" || value.toUpper() ==tr("通过"))
    {
        this->setStyleSheet("color: rgb(0, 255, 0);");
    }
    else
    {
        this->setStyleSheet("color: rgb(255,0, 0);");
    }
}

