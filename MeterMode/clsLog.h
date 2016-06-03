#ifndef CLSLOG_H
#define CLSLOG_H

#include <QThread>
#include <QTextStream>
#include <QObject>
#include <QFile>
class clsLog:public QThread
{
    Q_OBJECT
public:
    clsLog(QObject *parent);
    ~clsLog();
    void run();
    void setFileName(QString fileName);
    void writeLine(QString ln);
    void clearFile();
    void stop();
    void flushResult();

signals:
    void showErrorMessage(QString);
private:
    QString fileName;
    QFile *file;
    QTextStream *stream;
    bool openFile(QString fileName);
};

#endif // CLSLOG_H
