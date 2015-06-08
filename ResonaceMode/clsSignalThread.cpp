#include "clsSignalThread.h"
#include <QDebug>
clsSignalThread::clsSignalThread(QObject *parent) :
    QThread(parent)
{
    box = new clsControlBox();

    isStop =true;
}

clsSignalThread::~clsSignalThread()
{
    this->quit();
    this->wait();
}

void clsSignalThread::stop()
{
    box->stop();
    isStop =true;
}

clsControlBox *clsSignalThread::getControlBox()
{
    return this->box;
}

void clsSignalThread::setPass()
{
    box->setPass();
}

void clsSignalThread::setFail()
{
    box->setFail();
}

void clsSignalThread::setBDA()
{
    box->setBDA();
}

void clsSignalThread::resetBDA()
{
    box->resetBDA();
}

void clsSignalThread::run()
{
    if(box->getInit())
    {
        isStop =false;
        emit initOk();
    }
    else
        return;

    //qDebug()<<"control box in capture signal...";
    while(!isStop)
    {
        if(box->getInputSignal(0))
        {
            emit trigCaptured();
            this->msleep(20);
            //qDebug()<<"control box get trig signal...";
        }
        else
        {
            this->msleep(20);
        }
        //qDebug()<<"thread is running..";
    }
}
