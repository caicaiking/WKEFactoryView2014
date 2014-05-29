#include "UserfulFunctions.h"
#include <QStringList>
#include <QString>
#include <QTime>
#include <QStringListIterator>
#include <QCoreApplication>
#include <Plot.h>
#include <QDebug>
#include <Qwt/qwt_plot_curve.h>
#include <QMap>
#include <QTime>
using  std::vector;
UserfulFunctions::UserfulFunctions()
{
}


bool UserfulFunctions::IsDouble(const QStringList r)
{

    QStringListIterator it(r);
    bool ok=false;

    while(it.hasNext())
    {

        QString str=it.next();
        // qDebug()<<str;
        str.toDouble(&ok);
        if(!ok)
            return false;
    }

    return true;
}

bool UserfulFunctions::IsDouble(const QString r)
{
    bool ok;

    r.toDouble(&ok);
    return ok;
}


//value is like Z R Rdc etc.
QString UserfulFunctions::getSuffix(const QString &value)
{

    if (value=="Z" || value=="R" || value=="X" || value=="RDC")
        return QString("Ω");

    if(value=="A" || value==QString("θ"))
        return QString("°");

    if(value=="G" || value=="B" || value=="Y")
        return "S";

    if(value=="L")
        return "H";
    if(value=="C")
        return "F";
    if(value=="Q" || value=="D")
        return value;
    if(value.toUpper()=="FREQUENCY" || value.toUpper()==QObject::tr("频率"))
        return "Hz";
    if(value.toUpper()=="BIAS" || value.toUpper()==QObject::tr("偏置电压") || value.toUpper()==QObject::tr("电压信号"))
        return "V";
    if(value.toUpper()=="TIME" || value.toUpper()==QObject::tr("时间"))
        return "s";

    if(value.toUpper()=="BIASI" || value.toUpper()==QObject::tr("偏置电流"))
        return "A";
    if(value.toUpper()=="LEVELA" || value.toUpper()==QObject::tr("电流信号"))
        return "A";
    if(value.toUpper()=="LEVELV")
        return "V";

    return "";
}

QString UserfulFunctions::getName(const QString &value)
{
    if(value=="Z")
        return QObject::tr("阻抗");
    else if(value=="R")
        return QObject::tr("交流电阻");
    else if(value=="X")
        return QObject::tr("电抗");
    else if(value=="RDC")
        return QObject::tr("直流电阻");
    else if(value=="A" || value==QString("θ"))
        return QObject::tr("相位");
    else if(value=="G")
        return QObject::tr("电导");
    else if(value=="B")
        return QObject::tr("电纳");
    else if(value=="Y")
        return QObject::tr("导纳");
    else if(value=="L")
        return QObject::tr("电感");
    else if(value=="C")
        return QObject::tr("电容");
    else if(value=="Q")
        return QObject::tr("品质因素");
    else if(value=="D")
        return QObject::tr("损耗");
    else
        return QString::null;


}

void UserfulFunctions::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QList<QPointF> UserfulFunctions::getPlotCurveData(QwtPlotCurve *curve)
{
   QList<QPointF> data;
    for(int i=0; i< curve->data()->size();i++)
    {
        QPointF ptX= (QPointF)curve->data()->sample(i);
        data<<ptX;
    }
    return data;
}

QList<double> UserfulFunctions::resultPro(const QString &resultString)
{
    QStringList list= resultString.split(",");

    QList<double> myReturn;

    for(int i=0; i<list.count();i++)
    {
        myReturn<< list.at(i).toDouble();

//        if(myReturn.at(i)==0.0)
//            myReturn.insert(i,999999999);
    }
    return myReturn;

}

//将选点的曲线转换成一组像这样的形式（10.0K,<100Ohm,90Deg>）.
QMap <double, QPointF> UserfulFunctions::getPlotCurveData(const QList<PlotCurves> &curves,
                                                      int intSelect)
{

    QMap <double, QPointF> data;

    if(intSelect>=curves.length())
    {
        data.insert(0,QPointF(0.0,0.0));
        return data;
    }


   // qDebug()<< curves.at(intSelect).cur1->dataSize();
    data.clear();


    for(int i=0; i< curves.at(intSelect).cur1->data()->size();i++)
    {
        QPointF ptData;
        QPointF ptX=  curves.at(intSelect).cur1->data()->sample(i);
        ptData.setX(ptX.y());
        QPointF ptY=  curves.at(intSelect).cur2->data()->sample(i);
        ptData.setY(ptY.y());
        data.insert(ptX.x(),ptData);
    }

    return data;
}

double UserfulFunctions::Max(const QList<double> &arry)
{
    if(arry.length()==0)
        return 0.0;

    double max=arry.at(0);


    QListIterator<double> it(arry);

    while(it.hasNext())
    {
        double value= it.next();

        if(value>max)
            max=value;
    }

    return max;
}

double UserfulFunctions::Min(const QList<double> &arry)
{
    if(arry.length()==0)
        return 0.0;

    double min=arry.at(0);


    QListIterator<double> it(arry);

    while(it.hasNext())
    {
        double value= it.next();

        if(value<min)
            min=value;
    }

    return min;
}
double UserfulFunctions::Abs(double x)
{
    if(x<=0)
        return -1.0*x;
    else
        return x;
}

double UserfulFunctions::getNearItem(const QList<double> &arry, const double value,
                                  const bool isLog)
{
    if(arry.length()==0)
        return 0.0;

    if(value <= Min(arry))
        return Min(arry);

    if(value >=Max(arry))
        return Max(arry);

    QList<double> myArry=arry;
    double myValue=value;

    if(isLog)
    {
        myValue=log10(value);

        for(int i=0; i< myArry.length();i++)
        {
            myArry[i]=log10(myArry.at(i));
        }
    }

    double distance;

    distance= Abs(((double)myArry.last())-((double)myArry.first()));

    double myReturn;
    double realDistance= distance;

    int record=0;
    for(int i=0;i<myArry.length();i++)
    {
        double tmp;
        tmp= Abs(myArry.at(i)-myValue);

        if(realDistance>tmp)
        {
            myReturn=myArry.at(i);
            record=i;
            realDistance=tmp;
        }
    }
    return arry.at(record);
}
