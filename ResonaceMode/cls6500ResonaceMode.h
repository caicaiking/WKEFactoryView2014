#ifndef CLS6500RESONACEMODE_H
#define CLS6500RESONACEMODE_H
#include "clsResonaceMode.h"
#include <QString>
const QString RESONANCE =":METER:RESO:";
class cls6500ResonaceMode : public clsResonaceMode
{
    Q_OBJECT
public:

    void setStart(double value);
    void setStop(double value);
    void setSpeed(QString value);
    void setEqucct(QString value);
    QString trig();
    void setDepth(int value);
    void setCheckCp(bool value);
    double getStart();
    double getStop();
    QString getSpeed();
    int getDepth();
    bool getCheckCp();
    QString training();
    QString getEqucct();
signals:

public slots:

private:
    double start;
    double stop;



};

#endif // CLS6500RESONACEMODE_H
