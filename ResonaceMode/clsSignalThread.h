#ifndef CLSSIGNALTHREAD_H
#define CLSSIGNALTHREAD_H

#include <QThread>
#include "clsControlBox.h"
class clsSignalThread : public QThread
{
    Q_OBJECT
public:
    explicit clsSignalThread(QObject *parent = 0);
    ~clsSignalThread();
    void stop();
    clsControlBox * getControlBox();

    void setPass();
    void setFail();
    void setBDA();
    void resetBDA();


signals:
    void trigCaptured();
    void initOk();
public slots:

protected:
    void run();

private:
    clsControlBox *box;
    bool isStop;
};

#endif // CLSSIGNALTHREAD_H
