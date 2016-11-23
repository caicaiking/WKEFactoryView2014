#ifndef CLSRESONACEMODE_H
#define CLSRESONACEMODE_H

#include <QObject>

class clsResonaceMode:public QObject
{
    Q_OBJECT

public:
    virtual void  setStart(double value)=0;
    virtual void  setStop(double value)=0;
    virtual void setEqucct(QString value)=0;
    virtual QString trig()=0;
    virtual void setSpeed(QString)=0;
    virtual void setDepth(int)=0;
    virtual void setCheckCp(bool )=0;
    virtual QString training()=0;
    virtual double getStart()=0;
    virtual double getStop()=0;
    virtual QString getSpeed()=0;
    virtual int getDepth()=0;
    virtual bool getCheckCp()=0;
    virtual QString getEqucct()=0;

    //为了 兼容6500
    virtual QString getFmFn(){return "";}
    virtual QString getFsFp(){return "";}
    virtual double getCp1K(){return 0.0;}
    virtual double getKeff(){return 0.0;}


};

#endif // CLSRESONACEMODE_H
