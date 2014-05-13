在Connection文件夹中主要是包括了5个类1个接口


Interface 类：
    clsConnection
        |
        |   bool init()=0;
        |   void setAddress(QString address)=0;
        |   QString sendCommand(QString strCommand, bool hasReturn=false)=0;
        |   void disConnect()=0;

其他的派生类有:
    clsGpib:clsConnection   //GPIB 连接
    clsLan:clsConnection    //LAN 连接
    clsUsb:clsConnection    //USB连接

通过Factory（工厂）模式或者访问静态的方法来访问和构建这些连接
   clsConnection* connectionFactory::getConnection(QString)
   其中传递的参数可以选择 "GPIB","LAN","USB"


在通过singleton（单一）模式将连接变成了全局可以使用的连接
class Cnnt :public QObject


    static Cnnt* Instance();
        |
        | setConnectionType(QString connectionType);
        | setConnectionAddress(QString strAddress);
        | setUpConnection();
        | setUpConnection(QString connectionType, QString strAddress);
        | sendCommand(QString command, bool hasReturn);
        | QueryConnectionStatus(){return isConnectionInit;}

通过上面的Cnnt::Instance()->访问上面的这些方法，可以得到当前的连接状态，
也可以通过sendCommand去发送命令到仪器上面去。




