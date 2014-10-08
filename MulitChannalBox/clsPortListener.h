#ifndef PORTLISTENER_H_
#define PORTLISTENER_H_

#include <QObject>
#include <QSerialPort>
class clsPortListener : public QObject
{
Q_OBJECT
public:
    clsPortListener();
    bool getInit() const;
    int sendCommand(QString);

private:
    QSerialPort *port;
private slots:

private:
    QString strRes;
    bool init;

};


#endif /*PORTLISTENER_H_*/
