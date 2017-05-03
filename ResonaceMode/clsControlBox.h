#ifndef CLSCONTROLBOX_H
#define CLSCONTROLBOX_H

#include <QObject>
#include "AduHid.h"
#include <QMutex>
#include <QMutexLocker>
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
    void setPass();
    void setFail();
    void setBusy();
    void resetBusy();

    void emitBDA();
signals:
    void showStatus(QString);
public slots:
private slots:
    QString sendCommand(QString value, bool hasReturn=false);
    bool initDevice();
private:
    void *hDevice;

    bool blStop;
    bool isInit;

    void sleepMs(int svalue);
    QString shortRelay(int port);
    QString openRelay(int port);

    static QMutex lock;
};

#endif // CLSCONTROLBOX_H
