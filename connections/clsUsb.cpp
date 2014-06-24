#include "clsUsb.h"
#include <QDebug>
#include "UserfulFunctions.h"
clsUsb::clsUsb()
{
    blInit=false;
}

bool clsUsb::init()
{
    /* Begin by initializing the system*/
    status = viOpenDefaultRM(&defaultRM);
    if(status < VI_SUCCESS)
    {
        /* Error initializing VISA ... exiting */
        return false;
    }

    /* Find all usbTmc instruments in the system */
    status = viFindRsrc(defaultRM,
                        "USB[0-9]*::0x0B6A::0x5346::?*INSTR",
                        &findList,&retCount,desc);

    if (status < VI_SUCCESS)
    {
        /* Error finding resources ... exiting */
        viClose(defaultRM);
        return false;
    }

    if(retCount <=0 || retCount >=2)
    {
        /*Can't find any usbTmc instruments
         *|| more than one instrument.
        !*/
        return false;
    }

    /*Open session*/
    status = viOpen(defaultRM, desc,VI_NULL,VI_NULL,&instr);

    /*Set Attribute*/
    status = viSetAttribute(instr,VI_ATTR_TMO_VALUE,15000);
    if(status < VI_SUCCESS)
    {
        viClose(instr);
        return false;
    }

    /*Write query ID command*/
    status=viWrite(instr,ViBuf("*IDN?\n"),6,&retLen);
    if(status < VI_SUCCESS)
    {
        viClose(instr);
        return false;
    }

    /*Read id string data*/
    status = viRead(instr,buffer,MAX_CNT,&retLen);
    if(retLen>0)
        buffer[retLen]='\0';
    strResult=QString((const char*)buffer);

    if(strResult.contains("WAYNE"))
    {
        this->blInit=true;
        return true;
    }

    return false;

}

void clsUsb::setAddress(QString address)
{
    address ="";
}

QString clsUsb::sendCommand(QString strCommand, bool hasReturn, int /*waitDelay*/)
{
    if(!blInit)
        init();

    if(!blInit)
        return "";

    //change command formate.
    strCommand= strCommand.append('\n');
    const char*  stdString=strCommand.toStdString().c_str();
    ViBuf cmd=ViBuf(stdString);
    //send command
    status=viWrite(instr,cmd,strlen(stdString),&retLen);

    if(!hasReturn)
        return "";

//    if(waitDelay!=0)
//    {
//        UserfulFunctions::sleepMs(waitDelay*1000);
//    }

    /*Read id string data*/
    status = viRead(instr,buffer,MAX_CNT,&retLen);
    if(retLen>0)
        buffer[retLen]='\0';
    strResult=QString((const char*)buffer);

 //   qDebug()<< strResult;

    return strResult;
}

void clsUsb::disConnect()
{
    viClose(instr);
       qDebug("Usb Disconnected!");
}
