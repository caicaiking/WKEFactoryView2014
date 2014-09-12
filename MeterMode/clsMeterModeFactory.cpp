#include "clsMeterModeFactory.h"

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
QPointer<WKEMeterMode> clsMeterModeFactory::getFunction(FunctionType t, QString meterSeries)
{
    switch (t) {
    case MeterFunction:
        return new cls6440MeterMode();
        break;
    case UF:
        return new clsUserFunctionMeterMode();
        break;
    default:
        return 0;
        break;
    }
}
