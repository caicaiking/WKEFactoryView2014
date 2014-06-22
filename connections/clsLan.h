#ifndef CLSLAN_H
#define CLSLAN_H
#include <QTcpSocket>
#include <QObject>
#include "connections/clsConnection.h"
#define WK4300PORT 9760
#define WK6500PORT 2000
class clsLan : public clsConnection
{
public:
    clsLan();
    bool init();
    void setAddress(QString address);
    QString sendCommand(QString strCommand, bool hasReturn=false, int waitDaly=0);
    void disConnect();

    void sleep(unsigned int msec);
private:
    QString strIp;
    quint16 intPort;

    QTcpSocket *socket;
    bool blInit;

};

#endif // CLSLAN_H
