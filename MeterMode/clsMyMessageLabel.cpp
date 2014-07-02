#include "clsMyMessageLabel.h"
#include <QTime>
#include <QDate>
clsMyMessageLabel::clsMyMessageLabel(QWidget *parent) :
    QLabel(parent)
{
    timer = new QTimer(this);


}

void clsMyMessageLabel::showTime()
{
    this->setText(QDate::currentDate().toString("yyyy-MM-dd ")+ QTime::currentTime().toString("hh:mm:ss"));

    connect(timer, SIGNAL(timeout()),this,SLOT(showTimeLabel()));
    timer->setInterval(1000);
    timer->start();
}

void clsMyMessageLabel::showTimeLabel()
{
       this->setText(QDate::currentDate().toString("yyyy-MM-dd ")+ QTime::currentTime().toString("hh:mm:ss"));
}

void clsMyMessageLabel::showMessage(QString value, int time)
{

    this->setText(value);
    if(time ==0 )
        return;

    connect(timer, SIGNAL(timeout()),this,SLOT(clearMessage()));
    timer->setInterval(time *1000);
    timer->setSingleShot(true);
    timer->start();
}

void clsMyMessageLabel::clearMessage()
{
    this->setText("");
    timer->stop();
}
