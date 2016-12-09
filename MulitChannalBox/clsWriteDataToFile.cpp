#include "clsWriteDataToFile.h"
#include <QStringList>
#include "UserfulFunctions.h"
clsWriteDataToFile::clsWriteDataToFile()
{

}

clsWriteDataToFile::~clsWriteDataToFile()
{
    QList<int> keys = threads.keys();
    for(int i=0; i< keys.length(); i++)
    {
        threads[keys.at(i)]->stop();
    }
    UserfulFunctions::sleepMs(20);
}

void clsWriteDataToFile::startRecord()
{
    QList<int> keys = threads.keys();

    for(int i=0; i< keys.length(); i++)
    {
        //qDebug()<< i;
        threads[keys.at(i)]->start();
    }
}

void clsWriteDataToFile::stopRecord()
{
    QList<int> keys = threads.keys();
    for(int i=0; i< keys.length(); i++)
    {
        threads[keys.at(i)]->stop();
    }
}

void clsWriteDataToFile::setData(QString ch, QList<double> v)
{
    QStringList tmp ;
    QLocale loc;

    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QString sp =(QLocale().decimalPoint()=='.'?",":";");

    for(int i=0; i< v.length();i++)
    {
        tmp.append(loc.toString(v.at(i)));
    }
    threads[ch.toInt()]->setData(tmp.join(sp));
}

void clsWriteDataToFile::setChannel(QString value)
{
    this->channal = value;

    for(int i=0; i< channal.split(",").length(); i++)
    {
        QString ch = channal.split(",").at(i);
        if(!ch.isEmpty())
        {
            threads.insert(ch.toInt(), new clsWriteFileThread());
            threads[ch.toInt()]->setChannel(ch);
        }
    }
}

void clsWriteDataToFile::writeTitle(QString value)
{
    QList<int> keys = threads.keys();
    for(int i=0; i< keys.length(); i++)
    {
        threads[keys.at(i)]->setTitle(value);
    }
}

void clsWriteDataToFile::setPath(QString path)
{
    this->strPath = path;
    QList<int> keys = threads.keys();
    for(int i=0; i< keys.length(); i++)
    {
        threads[keys.at(i)]->setPath(strPath);
    }
}

