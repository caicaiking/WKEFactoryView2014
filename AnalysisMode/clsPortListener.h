#ifndef PORTLISTENER_H_
#define PORTLISTENER_H_

#include <QObject>
#include <QtExtSerialPort/qextserialport.h>

class clsPortListener : public QObject
{
Q_OBJECT
public:
    clsPortListener(const QString &portName);
    bool getInit() const;
    bool sendCommand(QString);

private:
    QextSerialPort *port;

signals:
    void reciveStr(QByteArray);
private slots:
    void onReadyRead();
    void onDsrChanged(bool status);
private:
    QString strRes;
    bool init;

};


#endif /*PORTLISTENER_H_*/
