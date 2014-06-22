#ifndef CLSCONTROLBOX_H
#define CLSCONTROLBOX_H

#include <QObject>
#include "AduHid.h"
class clsControlBox : public QObject
{
    Q_OBJECT
public:
    explicit clsControlBox(QObject *parent = 0);
    ~clsControlBox();
    void stop();
    bool getInit(){return isInit;}
    QString getInput(int port);
    bool getInputSignal(int port);
signals:

public slots:
private slots:
    QString sendCommand(QString value, bool hasReturn=false);
    bool initDevice();
private:
    void *hDevice;

    bool blStop;
    bool isInit;

    void sleepMs(int svalue);
};

#endif // CLSCONTROLBOX_H
