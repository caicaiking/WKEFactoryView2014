#ifndef CLSCONNECTSWBOX_H
#define CLSCONNECTSWBOX_H

#include <QObject>
#include "clsPortListener.h"
#include <QTimer>
#include <QStringList>
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
    int sendCommand(int chennal);
signals:

public slots:


private:

    clsPortListener *serialPort;
    QStringList chennals;
    int feedbackResult;
};



#endif // CLSCONNECTSWBOX_H
