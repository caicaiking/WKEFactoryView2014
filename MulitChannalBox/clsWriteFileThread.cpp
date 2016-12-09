#include "clsWriteFileThread.h"
#include <QTime>
#include <QDate>
#include <QApplication>
#include "UserfulFunctions.h"
clsWriteFileThread::clsWriteFileThread()
{

}

void clsWriteFileThread::setPath(QString path)
{
    this->strPath = path;
}

void clsWriteFileThread::setChannel(QString value)
{
    this->channal = value;
}

void clsWriteFileThread::setTitle(QString title)
{
    QTime strTime = QTime::currentTime();
    QDate strDate = QDate::currentDate();

    QString timeDate = strDate.toString("dd_MM_yyyy")+" " + strTime.toString("hh_mm_ss ");
    file = new QFile(strPath +timeDate+ channal +".txt");

    QLocale loc;

    QString sp =(QLocale().decimalPoint()=='.'?",":";");
    loc.setNumberOptions(QLocale::OmitGroupSeparator);

    if(file->open(QIODevice::Append))
    {

        this->strTitle = title;
        file->write(title.append(sp).append("Time").append("\n").toUtf8());
        file->flush();
    }
}

void clsWriteFileThread::setData(QString items)
{
    QString sp =(QLocale().decimalPoint()=='.'?",":";");
    QTime strTime = QTime::currentTime();
    QDate strDate = QDate::currentDate();

    QString timeDate = strDate.toString("dd/MM/yyyy")+" " + strTime.toString("hh:mm:ss");
    item.enqueue(items+ sp +timeDate);
}

void clsWriteFileThread::run()
{
    isRunning = true;

    while(isRunning)
    {
        if(!item.isEmpty())
        {
            QString tmpItem = item.dequeue();
            if(!tmpItem.isEmpty() && file->isWritable())
            {
                file->write(tmpItem.append("\n").toUtf8());
                file->flush();
                qApp->processEvents();
            }
        }
        qApp->processEvents();
        this->msleep(50);
    }

    while(!item.isEmpty())
    {
        QString tmpItem = item.dequeue();
        if(!tmpItem.isEmpty() && file->isWritable())
        {
            file->write(tmpItem.append("\n").toUtf8());
            file->flush();
        }
        qApp->processEvents();
    }
}

void clsWriteFileThread::stop()
{
    isRunning = false;
    qApp->processEvents();
    UserfulFunctions::sleepMs(20);
    if(file->isOpen())
    {
        file->flush();
        file->close();
    }
}

