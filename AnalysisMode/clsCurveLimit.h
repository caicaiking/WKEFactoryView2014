#ifndef CLSCURVELIMIT
#define CLSCURVELIMIT
#include <QColor>
#include "clsMeterLimit.h"
#include "clsSettings.h"
#include "UserfulFunctions.h"
class clsCurveLimit{
public:

    clsCurveLimit()
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

    bool status;
    void resetStatus()
    {
        status =true;
    }

    bool hasEnableLimit()
    {
        return blTraceALimit || blTraceBLimit;
    }

    QString getLimit1Show()
    {
        if(blTraceALimit)
            return cmlTraceALimit.showLimits();
        else
            return QObject::tr("没有设定");
    }

    QString getLimit2Show()
    {
        if(blTraceBLimit)
            return cmlTraceBLimit.showLimits();
        else
            return QObject::tr("没有设定");

    }

    void compareValue(const double item1,const double item2)
    {
        if(blTraceALimit)
            status= status && cmlTraceALimit.comparaValue(item1);
        if(blTraceBLimit)
            status = status && cmlTraceBLimit.comparaValue(item2);
    }

    void readSettings()
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


    void writeSettings()
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

    QString item1,item2;

    bool blTraceALimit,blTraceBLimit,blPassSound,blFailSound;
    QColor traceAUp, traceBUp, traceADown,traceBDown;
    clsMeterLimit cmlTraceALimit,cmlTraceBLimit;
    int intSlect; //用于标识是否为多限制模式

};

#endif // CLSCURVELIMIT



