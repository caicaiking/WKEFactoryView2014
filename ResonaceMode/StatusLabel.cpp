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
        emit statusChange(BUSY);
        break;
    case PASS:
        this->setText(tr("通过"));
        this->setStyleSheet(" background-color: rgb(0, 255, 0);border-radius: 6px");
        emit statusChange(PASS);
        break;
    case FAIL:
        this->setText(tr("失败"));
        this->setStyleSheet("background-color: rgb(255, 0, 0);border-radius: 6px");
        emit statusChange(FAIL);
        break;
    case IDEL:
        this->setText(tr("空闲"));
        this->setStyleSheet("background-color: rgb(255, 104, 32);border-radius: 6px");
        emit statusChange(IDEL);
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
        emit statusChange(PASS);
    }
    else
    {
        this->setText(tr("失败"));
        this->setStyleSheet("background-color: rgb(255, 0, 0);border-radius: 6px");
        emit statusChange(FAIL);
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
