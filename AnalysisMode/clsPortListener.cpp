#include "clsPortListener.h"
#include <QtDebug>

clsPortListener::clsPortListener(const QString &portName)
{

    init = false;
    this->port = new QextSerialPort(portName, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD2400);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    if(this->port->isOpen())
        this->port->close();

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        qDebug() << "listening for data on" << port->portName();
        init =true;
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
        init = false;
    }
}

void clsPortListener::onReadyRead()
{
    QByteArray bytes;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
//    qDebug() << "Recive data size:\t" << bytes.size() << "Data is: " << bytes;

//    for(int i =0; i < bytes.size();i++)
//    {
//        int value = (unsigned char)bytes[i];
//        qDebug() << "\t\tRecieve byte " << i<< ":\t" << value;
//    }

    emit reciveStr(bytes);

}

void clsPortListener::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}
bool clsPortListener::getInit() const
{
    return init;
}

bool clsPortListener::sendCommand(QString value)
{
    if(!init)
        return false;

    value =value +"\r";

   int byteWriten = port->write(value.toStdString().c_str());

   if(byteWriten ==-1)
       qDebug() <<"Write Date to Rs232 Error!";

   if(byteWriten == value.length())
       return true;
   else
       return false;
}



