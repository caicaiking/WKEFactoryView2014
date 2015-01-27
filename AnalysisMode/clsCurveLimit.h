#ifndef CLSCURVELIMIT
#define CLSCURVELIMIT
#include <QColor>
#include "clsMeterLimit.h"
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

    }


    bool blTraceALimit,blTraceBLimit;
    QColor traceAUp, traceBUp, traceADown,traceBDown;
    clsMeterLimit cmlTraceALimit,cmlTraceBLimit;

};

#endif // CLSCURVELIMIT

