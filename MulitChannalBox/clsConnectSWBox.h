#ifndef CLSCONNECTSWBOX_H
#define CLSCONNECTSWBOX_H

#include <QObject>
#include "clsPortListener.h"
#include <QTimer>
#include <QStringList>
#include "Globle.h"
class clsConnectSWBox : public QObject
{
    Q_OBJECT
private:
    explicit clsConnectSWBox(QObject *parent = 0);
    static clsConnectSWBox *_instance;

public:
    static clsConnectSWBox *Instance();
    bool initSerialPort();
    void closeSeriesPort();
    int sendCommand(QString value);
    int selectChannel(int chennal);

    int setChannelStatus(int channel, Status status);
    int setAllChannelIdel();
    int setTerminal(int value);
    int turnOffAllLight();
    int setOnlyOneOrangeLEDON(int channel);
    void setLedOff(bool value);
signals:

public slots:


private:

    clsPortListener *serialPort;
    QStringList chennals;
    int feedbackResult;
    bool isLedOff;


};



#endif // CLSCONNECTSWBOX_H
