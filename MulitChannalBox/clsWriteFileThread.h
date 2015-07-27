#ifndef CLSWRITEFILETHREAD_H
#define CLSWRITEFILETHREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QFile>
class clsWriteFileThread : public QThread
{
public:
    clsWriteFileThread();

    void setPath(QString path);
    void setChannel(QString value);
    void setTitle(QString title);
    void setData(QString items);
    void run();
    void stop();
private:
    QString strPath;
    QString channal;
    QString strTitle;
    bool isRunning;
    QFile * file;
    QQueue<QString> item;
};

#endif // CLSWRITEFILETHREAD_H
