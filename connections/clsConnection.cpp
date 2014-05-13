#include "connections/clsConnection.h"
#include "connections/clsGpib.h"
#include "connections/clsUsb.h"
#include "connections/clsLan.h"
connectionFactory::connectionFactory()
{
}

/*通过工厂模式来获取连接的接口实例。
 *
 *V1.0
 *!*/

clsConnection *connectionFactory::getConnection(QString strConnect)
{
    if(strConnect.toUpper()=="GPIB")
    {
        return new clsGpib;
    }
    else if(strConnect.toUpper()=="USB")
    {
        return new clsUsb;
    }
    else if(strConnect.toUpper()=="LAN")
    {
        return new clsLan;
    }
    else
    {
        return (clsConnection *)0;
    }

}

/*可以通过工厂模式去获取GPIB，也可以通过调用这个程序来获取
 *
 *V1.0
 *!*/
clsConnection *connectionFactory::getGpib()
{
    return new clsGpib;
}


/*可以通过工厂模式去获取USB，也可以通过调用这个程序来获取
 *
 *V1.0
 *!*/
clsConnection *connectionFactory::getUsb()
{
    return new clsUsb;
}


/*可以通过工厂模式去获取LAN，也可以通过调用这个程序来获取
 *
 *V1.0
 *!*/
clsConnection *connectionFactory::getLan()
{
    return new clsLan;
}
