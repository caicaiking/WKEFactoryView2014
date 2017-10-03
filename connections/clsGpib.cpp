/**
 *这个是用于GPIB连接的类其中包括了GPIB初始化，设置GPIB地址默认是6，发送命令和解除连接。
 *
 *这个类只能连接一台GPIB设备，多余其他的GPIB设备不能使用这个类来操作。
 *
 *这个GPIB连接类只能用于连接WK的仪器使用，因为里面限定了以“WAYNE”开头的仪器设备返回的字符串。
 *
 *作者：蔡同松
 *
 *版本：V1.0
 *
 *时间：2013-2-22
 *
 *!*/

#include "clsGpib.h"
#include "clsConnection.h"
#include "gpib488.h"
#include <QStringList>
#include <QDebug>


clsGpib::clsGpib()
{
    this->intAddress=6;
    this->blInit=false;

}

bool clsGpib::init()
{
    //    ibdev(0,intAddress,0,10,1,0);

    //    Send(0,intAddress,"*IDN?",strlen("*IDN?"),DABend);
    //    char data[50];
    //    Receive (0,intAddress, data, 50, STOPend);

    //    QString strData = QString(data);
    //    QStringList strDataList = strData.split('\n');
    //    if(strDataList.count()>1)
    //    {
    //        strData =strDataList[0];
    //    }
    //    else
    //        strData="";

    //    qDebug()<< strData;
    //    if(strData.isEmpty())
    //    {
    //        blInit=false;
    //    }
    //    else
    //    {
    //        if(strData.contains("WAYNE") || strData.contains("KUWAKI"))
    //        {

    //            blInit = true;
    //        }
    //        else
    //        {
    //            blInit = false;
    //        }
    //    }


    //New method to do gpib control
    intBoardDescriptor = ibdev(GPIBBOARD,intAddress,0,T10s,1, 0x0c0a);

    if(ibsta & ERR)
    {
        blInit = false;
        qDebug()<<"ibdev returns ibsta error (ibsta = 0x" << (hex) << ibsta << "), problems while communicating to PC interface board";
        return blInit;
    }
    else
    {
        if(-1 == intBoardDescriptor)
        {
            qDebug()<<"ibdev error (Bd - Board descriptor error), problems while communicating to PC interface board";
            blInit = false;
            return blInit;
        }
        else
        {
            ibconfig(GPIBBOARD, IbcAUTOPOLL,false);
            ibconfig(GPIBBOARD, IbcHSCableLength, false);
            ibconfig(GPIBBOARD, IbcTIMING,1);
            blInit = true;
        }
    }
    return this->blInit;
}



void clsGpib::setAddress(QString address)
{
    this->intAddress = address.toInt();

    if(address.isEmpty() || this->intAddress==0)
        this->intAddress =6;

}

QString clsGpib::sendCommand(QString strCommand, bool hasReturn, int /*waitDelay*/)
{

    if(!blInit)
    {
        init();
    }

    if(!blInit)
        return "";

    //strCommand = strCommand.append('\n');
    const char *cmd;
    std::string xx= strCommand.toStdString();
    cmd = xx.c_str();


    unsigned int cmdLength;

    cmdLength=strlen(cmd);

    //    Send(0,intAddress,cmd,cmdLength,DABend);

    //    if(Ibcnt()!= cmdLength || Ibsta() & ERR)
    //             return "";

    //    if(!hasReturn)
    //        return "";

    //    char buffer[129];
    //    const int bufsize=128;

    ////    if(waitDelay!=0)
    ////    {
    ////        UserfulFunctions::sleepMs(waitDelay*1000);
    ////    }

    //    Receive (0,intAddress, buffer, bufsize, STOPend);


    //    while ( (ibsta & CMPL+ERR) == 0);

    //    if ((Ibcnt()==bufsize) || (Ibcnt()==0 )|| (Ibsta()&ERR))
    //        return "";
    //    else
    //    {
    //        buffer[Ibcnt()-1] = '\0';    /* Terminate string */
    //    }

    //    QString str= QString(buffer);


    //    if(!str.isEmpty())  //在前一段时间发现，6500的返回值前面多了一个‘N’这个非常奇怪。
    //    {
    //        if(str.at(0) =='N')
    //            str= str.remove(0,1);
    //    }

    // qDebug()<< str;

    // 2017 / 1 /9
    //New Method to do gpib control
    ibwrt(this->intBoardDescriptor, cmd,cmdLength);

    if(hasReturn == false)
        return "";


    short int status;
    char pollStatus;

    //    WaitSRQ(GPIBBOARD, &status);
    //    if(!status)
    //    {
    //        qDebug()<<"Error while Wati SRQ";
    //        return "";
    //    }

    //    if(ibsta & ERR)
    //    {
    //        qDebug()<<"ibsta shows error under WaitSRQ";
    //        return "";
    //    }

    //    status = ibrsp(this->intBoardDescriptor, & pollStatus);
    //    if(ibsta & ERR)
    //    {
    //        qDebug()<<"ibsta shows error under ibrsp";
    //        return "";
    //    }

    char buffer[128];
    const int bufsize=128;

    //    if(pollStatus & 0x40)
    //    {
    //Bit 6 of the pollStatus indicates whatever the device has data or not
    (void) ibrd(this->intBoardDescriptor, buffer,bufsize);

    if(ibsta & ERR)
    {
        qDebug()<<"Read data Error";
        return "";
    }
    else
    {
        buffer[ibcntl-1] = 0x00;
        QString str= QString(buffer);

        if(!str.isEmpty())  //在前一段时间发现，6500的返回值前面多了一个‘N’这个非常奇怪。
        {
            if(str.at(0) =='N')
                str= str.remove(0,1);
        }

        return str;
    }

    //    }





}

void clsGpib::disConnect()
{
    short addres[]={(short) intAddress};
    EnableLocal(0,addres);
    blInit=false;
    qDebug("Gpib Disconnected!");
}
