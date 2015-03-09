#include "StatusLabel.h"

StatusLabel::StatusLabel(QWidget *parent) :
    QLabel(parent)
{
    this->st=IDEL;
    setStatus(st);
}

void StatusLabel::setStatus(Status value)
{
    switch (value) {
    case BUSY:
        this->setText(tr("忙碌"));
        this->setStyleSheet("background-color: rgb(255, 255, 0);border-radius: 6px");
        break;
    case PASS:
        this->setText(tr("通过"));
        this->setStyleSheet(" background-color: rgb(0, 255, 0);border-radius: 6px");
        break;
    case FAIL:
        this->setText(tr("失败"));
        this->setStyleSheet("background-color: rgb(255, 0, 0);border-radius: 6px");
        break;
    case IDEL:
        this->setText(tr("空闲"));
        this->setStyleSheet("background-color: rgb(183, 255, 172);border-radius: 6px");
        break;
    default:
        break;
    }
    this->st=value;
    qApp->processEvents();
}

void StatusLabel::setStatus(bool value)
{
    if(value)
    {
        this->setText(tr("通过"));
        this->setStyleSheet(" background-color: rgb(0, 255, 0);border-radius: 6px");
    }
    else
    {
        this->setText(tr("失败"));
        this->setStyleSheet("background-color: rgb(255, 0, 0);border-radius: 6px");
    }

    this->st=IDEL;
    qApp->processEvents();
}


Status StatusLabel::getStatus()
{
    return this->st;
}
void StatusLabel::mouseReleaseEvent(QMouseEvent *)
{
    emit Clicked();

}
