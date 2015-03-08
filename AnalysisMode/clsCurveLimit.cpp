#include "clsCurveLimit.h"
#include "UserfulFunctions.h"

clsCurveLimit::~clsCurveLimit()
{

}



clsCurveLimit::clsCurveLimit()
{
    traceAUp =Qt::green;
    traceADown = Qt::red;
    traceBUp = Qt::darkGreen;
    traceBDown = Qt::darkRed;
    blTraceALimit=false;
    blTraceBLimit=false;
    blPassSound=true;
    blFailSound=true;
}

void clsCurveLimit::resetStatus()
{
    status =true;
}

bool clsCurveLimit::hasEnableLimit()
{
    return blTraceALimit || blTraceBLimit;
}

QString clsCurveLimit::getLimit1Show()
{
    if(blTraceALimit)
        return cmlTraceALimit.showLimits(UserfulFunctions::getSuffix(item1));
    else
        return QObject::tr("没有设定");
}

QString clsCurveLimit::getLimit2Show()
{
    if(blTraceBLimit)
        return cmlTraceBLimit.showLimits(UserfulFunctions::getSuffix(item2));
    else
        return QObject::tr("没有设定");

}

void clsCurveLimit::compareValue(const double item1, const double item2)
{
    if(blTraceALimit)
        status= status && cmlTraceALimit.comparaValue(item1);
    if(blTraceBLimit)
        status = status && cmlTraceBLimit.comparaValue(item2);
}

void clsCurveLimit::readSettings()
{
    clsSettings settings;

    QString strNode="CurveLimit/";
    settings.readSetting(strNode+"blTraceALimit",this->blTraceALimit);
    settings.readSetting(strNode+"blTraceBLimit",this->blTraceBLimit);
    settings.readSetting(strNode+"traceAUpColor",this->traceAUp);
    settings.readSetting(strNode+"traceADownColor",this->traceADown);
    settings.readSetting(strNode+"traceBUpColor",this->traceBUp);
    settings.readSetting(strNode+"traceBDownColor",this->traceBDown);

    settings.readSetting(strNode+"passSound",this->blPassSound);
    settings.readSetting(strNode+"failBSound",this->blFailSound);
    settings.readSetting(strNode+"intSelect",this->intSlect);
    QString tmp;
    settings.readSetting(strNode+"TraceALimit",tmp);
    if(!tmp.isEmpty())
        cmlTraceALimit.setString(tmp);
    settings.readSetting(strNode+"TraceBLimit",tmp);

    if(!tmp.isEmpty())
        cmlTraceBLimit.setString(tmp);

}

void clsCurveLimit::writeSettings()
{
    clsSettings settings;

    QString strNode="CurveLimit/";
    settings.writeSetting(strNode+"blTraceALimit",this->blTraceALimit);
    settings.writeSetting(strNode+"blTraceBLimit",this->blTraceBLimit);
    settings.writeSetting(strNode+"traceAUpColor",this->traceAUp);
    settings.writeSetting(strNode+"traceADownColor",this->traceADown);
    settings.writeSetting(strNode+"traceBUpColor",this->traceBUp);
    settings.writeSetting(strNode+"traceBDownColor",this->traceBDown);
    settings.writeSetting(strNode+"passSound",this->blPassSound);
    settings.writeSetting(strNode+"failBSound",this->blFailSound);
    settings.writeSetting(strNode+"TraceALimit",cmlTraceALimit.toString());
    settings.writeSetting(strNode+"TraceBLimit",cmlTraceBLimit.toString());
    settings.writeSetting(strNode+"intSelect",this->intSlect);
}

void clsCurveLimit::setItem(QString item1, QString item2)
{
    this->item1=item1;
    this->item2=item2;
}
