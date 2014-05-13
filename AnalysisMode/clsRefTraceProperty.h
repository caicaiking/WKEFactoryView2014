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
    curveProperty(int index=1,QColor traceA=Qt::red,QColor traceB=Qt::darkRed,
                  QString title=QString::null,bool isOn=false)
    {
        this->index=index;
        this->traceA=traceA;
        this->traceB=traceB;
        this->title=title;
        this->isOn=isOn;


    }

    QString toString()
    {
        return QString("%1\t%2\t%3").arg(index).arg(title).arg(isOn);
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
        return false;
    }

    void setIndex(const int value){this->index=value;}
    ~curveProperty(){}
};


#endif // CLSREFTRACEPROPERTY_H
