#include "clsMeterModeFactory.h"
#include "cls4100MeterMode.h"
#include "cls6440MeterMode.h"
#include "cls6500MeterMode.h"
#include <QMessageBox>

clsMeterModeFactory::clsMeterModeFactory()
{
}

/*!
 * \brief clsMeterModeFactory::getFunction
 * \param t 是那个类型
 * \param meterSeries 用于返回连接的哪一块表
 * \return 返回一个Meter或者function
 * 返回表
 */
WKEMeterMode *clsMeterModeFactory::getFunction(QString meterSeries)
{

    if(meterSeries=="6440")
        return new cls6440MeterMode();
    else if(meterSeries =="4300")
        return new cls4100MeterMode();
    else if(meterSeries =="6500")
        return new cls6500MeterMode();
    else
    {
        QMessageBox::information(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("Not support this instrument now! \n Comming soon!"));
        qApp->exit(0);
        return 0;
    }
}
