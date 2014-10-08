#ifndef CLSCONNECTSWBOX_H
#define CLSCONNECTSWBOX_H

#include <QObject>
#include "clsPortListener.h"
#include <QTimer>
class clsConnectSWBox : public QObject
{
    Q_OBJECT
private:
    explicit clsConnectSWBox(QObject *parent = 0);
    static clsConnectSWBox *_instance;

public:
    static clsConnectSWBox *Instance();
    bool initSerialPort();
    int sendCommand(QString value);
signals:

public slots:


private:

    clsPortListener *serialPort;

    int feedbackResult;
};



#endif // CLSCONNECTSWBOX_H
