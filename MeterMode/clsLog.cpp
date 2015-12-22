#include "clsLog.h"
#include <QFile>
clsLog::clsLog(QObject *parent):QThread(parent)
{
    stream = NULL;
    file =NULL;
}

clsLog::~clsLog()
{
    this->stop();
}

void clsLog::run()
{
    openFile(fileName);
}

bool clsLog::openFile(QString fileName)
{
    file = new QFile(fileName);

    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit showErrorMessage(tr("无法写Data Log文件：%1").arg(fileName));
        return false;
    }

    stream = new QTextStream(file);
    stream->setCodec("GBK");
    return true;
}


void clsLog::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void clsLog::writeLine(QString ln)
{
    if((file ==NULL) || (stream==NULL))
    {
       openFile(fileName);
    }


    (*stream)<<ln<<"\n";
    stream->flush();
}

void clsLog::clearFile()
{
    if(stream!=NULL)
        stream->flush();

    if(file!=NULL)
        file->close();

    if(QFile::exists(fileName))
        QFile::remove(fileName);

    file=NULL;
    stream=NULL;
}

void clsLog::stop()
{
    if(stream!=NULL)
        stream->flush();

    if(file!=NULL)
        file->close();
}

void clsLog::flushResult()
{
    if(stream !=NULL)
        stream->flush();
}

