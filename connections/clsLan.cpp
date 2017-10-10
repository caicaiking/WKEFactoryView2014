#include "clsLan.h"
#include <QTcpSocket>
#include <QStringList>

#include <QApplication>
#include <QDataStream>
#include <QTime>
#include <QDebug>
clsLan::clsLan()
{
    this->blInit=false;
    this->strIp="192.168.1.199";
    this->intPort=WK4300PORT;
}

bool clsLan::init()
{
    socket  = new QTcpSocket(this);
    socket->connectToHost(this->strIp,this->intPort,QIODevice::ReadWrite);
    const int Timeout=5*1000;
    //qDebug("State:%d ",socket->state());
    // State: 2; 2代表ConnectingState，正确的应该为3（ConnectedState）
    if(!socket->waitForConnected(Timeout))
    {
        this->blInit=false;
        return false;
    }
    //qDebug("State:%d ",socket->state());
    // State: 3（ConnectedState）正确
    this->blInit=true;
    // connect(socket,SIGNAL(readyRead()),this,SLOT(read()));
    return true;
}

//Address formate "192.168.1.45,6500"
/**
 *这儿如果是4300传入数据的话，可以直接 用数据格式为“192.168.1.45(,4300)”
 *
 *?*/
void clsLan::setAddress(QString address)
{
    QStringList list = address.split(",");
    //qDebug()<< list;
    if(list.length()==1)
    {
        this->strIp = (QString)list.at(0);
        this->intPort=WK4300PORT;
    }
    else if(QString(list.at(1)).contains("6500"))
    {
        this->strIp=(QString)list.at(0);
        this->intPort=WK6500PORT;
    }
    else
    {
        this->strIp=(QString)list.at(0);
        this->intPort=WK4300PORT;
    }

}

QString clsLan::sendCommand(QString strCommand, bool hasReturn, int waitDaly)
{

    if(!blInit)
    {
        qDebug()<<"Go to init function";
        init();
    }

    if(socket->state() == QAbstractSocket::UnconnectedState)
    {
        qDebug()<<"Init function failed, to to init function again";
        socket->disconnectFromHost();
        init();
    }

    if(!blInit)
        return "";

    //if(this->intPort == WK4300PORT)
        strCommand = strCommand.append("\r\n");


    socket->write(strCommand.toUtf8());

    socket->waitForBytesWritten(5000);

    if((this->intPort == WK4300PORT) && (!hasReturn))
        return "";


    if(this->intPort == WK4300PORT) //对于4300 我只能这样写了 6500 的做法完全不一样
    {

        if(socket->waitForReadyRead(3000))
        {
            char buff[200];
            int byte = socket->readLine(buff,200);
            if(byte>=0)
                buff[byte]='\0';

            QString res = QString(buff);

            if(socket->state() != QAbstractSocket::ConnectedState)
            {
                blInit = false;
                socket->disconnectFromHost();
            }

            return res.remove('\n');
        }
        else
            return "";
    }
    else
    {
        //qDebug()<<waitDaly;
        if(waitDaly==0)
            socket->waitForReadyRead(10000);
        else
            socket->waitForReadyRead(waitDaly*3000);

        QByteArray resBuff = socket->readAll();

        QString res = QString(resBuff);

        if(socket->state() != QAbstractSocket::ConnectedState)
        {
            blInit = false;
            socket->disconnectFromHost();
        }

        return res.remove('\n');
    }

}

void clsLan::disConnect()
{
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState ||
            socket->waitForDisconnected(1000))
        qDebug("Lan Disconnected!");
}

void clsLan::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
