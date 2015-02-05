#ifndef CLSCURVELIMIT
#define CLSCURVELIMIT
#include <QColor>
#include "clsMeterLimit.h"
#include "clsSettings.h"

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
    }


    bool blTraceALimit,blTraceBLimit,blPassSound,blFailSound;
    QColor traceAUp, traceBUp, traceADown,traceBDown;
    clsMeterLimit cmlTraceALimit,cmlTraceBLimit;

};

#endif // CLSCURVELIMIT



