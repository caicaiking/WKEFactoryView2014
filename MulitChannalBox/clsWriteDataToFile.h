#ifndef CLSWRITEDATATOFILE_H
#define CLSWRITEDATATOFILE_H

#include <QMap>
#include <QString>
#include "clsWriteFileThread.h"
class clsWriteDataToFile
{
public:
    clsWriteDataToFile();
    ~clsWriteDataToFile();

    void setChannel(QString value); //1
    void setPath(QString path);     //2
    void writeTitle(QString);       //3
    void startRecord();             //4
    void stopRecord();              //5
    void setData(QString ch,QList<double> v);  //6
private:
    QString channal;
    QString strPath;

    QMap<int, clsWriteFileThread*> threads;
};

#endif // CLSWRITEDATATOFILE_H
