#ifndef CLSREFTRACEPROPERTY_H
#define CLSREFTRACEPROPERTY_H
#include <QColor>
#include <QString>

class curveProperty{
public:
    bool isOn;
    QColor traceA;
    QColor traceB;
    QString title;
    int index;

    int refTraceType;
    QList<double> x1,y1,y2;


    curveProperty(int index=0,QColor traceA=Qt::red,QColor traceB=Qt::darkRed,
                  QString title=QString::null,bool isOn=false,int refTraceType=0,
                  QList<double> x=QList<double>(), QList<double>y1=QList<double>(), QList<double>y2=QList<double>())
    {
        this->index=index;
        this->traceA=traceA;
        this->traceB=traceB;
        this->title=title;
        this->isOn=isOn;
        this->refTraceType= refTraceType;
        this->x1=x;
        this->y1=y1;
        this->y2=y2;
    }

    QString toString()
    {
        return QString("%1 %2 %3").arg(index).arg(title).arg(isOn);
    }

    //忽略了标题的变化，对曲线的影响变化。
    bool operator !=(curveProperty value)
    {
        if(value.index != this->index)
            return true;
        if(value.traceA != this->traceA)
            return true;
        if(value.traceB != this->traceB)
            return true;
        if(value.isOn !=this->isOn)
            return true;
        if(value.title!=this->title)
            return true;

        if(value.refTraceType  != this->refTraceType)
            return true;

        if(value.x1!= this->x1)
            return true;

        if(value.y1!= this->y1)
            return true;

        if(value.y2 != this->y2)
            return true;

        return false;
    }

    void setIndex(const int value){this->index=value;}
    ~curveProperty(){}
};


#endif // CLSREFTRACEPROPERTY_H
