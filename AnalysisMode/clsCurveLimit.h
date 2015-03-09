#ifndef CLSCURVELIMIT_H
#define CLSCURVELIMIT_H

#include <QColor>
#include "UserfulFunctions.h"
#include "clsMeterLimit.h"
#include "clsSettings.h"
#include <QObject>
class clsCurveLimit{
public:

    clsCurveLimit();

    ~clsCurveLimit();

    bool status;
    void resetStatus();

    bool hasEnableLimit();

    QString getLimit1Show();

    QString getLimit2Show();

    void compareValue(const double item1,const double item2);

    void readSettings();


    void writeSettings();
    void setItem(QString item1, QString item2);

    QString item1,item2;

    bool blTraceALimit,blTraceBLimit,blPassSound,blFailSound;
    QColor traceAUp, traceBUp, traceADown,traceBDown;
    clsMeterLimit cmlTraceALimit,cmlTraceBLimit;
    int intSlect; //用于标识是否为多限制模式
};

#endif // CLSCURVELIMIT_H
