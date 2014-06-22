#include "clsRuningSettings.h"

#include <QMutex>
#include <QReadWriteLock>
#include "clsConnection.h"
#include <QVector3D>


class clsRS::PrivateData
{
public:

    PrivateData()
    {
        connectType ="GPIB";
        gpibAddress ="6";
        ipAddress="192.168.1.101";
        blInit=false;
     }
    ~PrivateData(){}

    QString getConnectType()
    {
        return connectType;
    }

    void createConnection()
    {
        cont = connectionFactory::getConnection(connectType);
        cont->setAddress(strAddress);
        blInit = cont->init();
    }

    void disConn()
    {
        if(cont->init())
            cont->disConnect();
    }

    bool isContInit()
    {
       return cont->init();
    }

    QString sendCommand(QString strCommnad, bool hasReturn, int delay=0)
    {
        if(!blInit)
            return "";

        mutex.lock();
        QString strReturn="";

        const bool isLocked = lock.tryLockForWrite();

        if(isLocked)
        {
            lock.unlock();
            strReturn= cont->sendCommand(strCommnad,hasReturn,delay);
        }
        mutex.unlock();
        mutex.unlock();
        return strReturn;
    }



    QReadWriteLock lock;
    QMutex mutex;

    QString strAddress;
    QString connectType;
    QString gpibAddress;
    QString ipAddress;

    bool blInit;


private:
    clsConnection *cont;
};

clsRS::clsRS()
{
   data = new PrivateData;

}


clsRS &clsRS::getInst()
{
    static clsRS settings;
    return settings;
}

QString clsRS::getConnectionType()
{
    return data->connectType;
}



void clsRS::setConnectType(QString value)
{
    data->mutex.lock();

    const bool isLocked = data->lock.tryLockForWrite();

    if(isLocked)
    {
        data->connectType = value;
        data->lock.unlock();
    }
    data->mutex.unlock();
}

QString clsRS::getIp()
{
    return data->ipAddress;

}

void clsRS::setIp(QString value)
{
    data->mutex.lock();

    const bool isLocked = data->lock.tryLockForWrite();

    if(isLocked)
    {
        data->ipAddress = value;
        data->strAddress=value;
        data->lock.unlock();
    }
    data->mutex.unlock();
}

void clsRS::setAddress(QString value)
{
    setIp(value);
}

QString clsRS::getGpibAddress()
{
    return data->gpibAddress;
}

void clsRS::setGpibAddress(QString value)
{
    data->mutex.lock();

    const bool isLocked = data->lock.tryLockForWrite();

    if(isLocked)
    {
        data->gpibAddress = value;
        data->strAddress=value;
        data->lock.unlock();
    }
    data->mutex.unlock();
}

bool clsRS::initConnection()
{
    data->createConnection();
    return data->blInit;
}

void clsRS::disConnect()
{
    data->disConn();
}

QString clsRS::sendCommand(QString strCommand, bool hasReturn)
{
    return  data->sendCommand(strCommand,hasReturn,0);
}

QString clsRS::sendCommand(QString strCommand, bool hasReturn, int delayTime)
{
    return  data->sendCommand(strCommand,hasReturn,delayTime);
}

bool clsRS::isConnectionInit()
{
    return data->isContInit();
}


clsRS::~clsRS()
{
    delete data;
}
