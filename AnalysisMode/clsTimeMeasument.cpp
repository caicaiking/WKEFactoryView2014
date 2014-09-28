#include "clsTimeMeasument.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "Globle.h"
#include <QTime>
#include <QMessageBox>
void clsTimeMeasument::setMin(double value)
{
    this->dblMin=value;
}

void clsTimeMeasument::setMax(double value)
{
    this->dblMax=value;
}

void clsTimeMeasument::setItems(const QString &item1, const QString &item2)
{
    this->testItem1=item1;
    this->testItem2=item2;

}

void clsTimeMeasument::setPoint(QList<double> *point)
{
    this->points = point;
}

void clsTimeMeasument::setItemsAndPoints(const QString &item1, const QString &item2,
                                         QList<double> *point)
{
    setItems(item1,item2);
    setPoint(point);
}

void clsTimeMeasument::trig()
{
    times.clear();
    item1.clear();
    item2.clear();
    isStop=false;

    int dblInterval = (points->last()-points->first())*1000/(points->length());

    if(dblInterval < 80)
    {
       QString text=(tr("<font size=\"3\" color=\"red\">时间间隔太短，不支持%1点.").arg(points->length()));

       QMessageBox::warning(0,tr("FactoryView 2014"),text,QMessageBox::Ok);
        return;
    }

    startTime= QTime::currentTime();

    QString strRes=meter->trig();
    QList<double> list=  UserfulFunctions::resultPro(strRes);

    times<<points->at(0);
    item1 << list.at(0);
    item2 << list.at(1);
    QTime tmp=startTime;
    int i=1;
    while(startTime.msecsTo(QTime::currentTime())<
          (int)(( points->last()-points->at(0))*1000.0) && !isStop)
    {

        qApp->processEvents();


        if(startTime.msecsTo(QTime::currentTime()) >=points->at(i)*1000.0)
        {
           // qDebug()<< points->at(i);
            times<<points->at(i);
            strRes=meter->trig();
            list=  UserfulFunctions::resultPro(strRes);
            item1 << list.at(0);
            item2 << list.at(1);
            tmp=QTime::currentTime();
            qApp->processEvents();
            updatePlot();
            double progress = times.last()/(dblMax-dblMin)*100;
            emit showProgress(progress);
            i++;

            if(i>points->length()-1)
                goto GoOut;
        }
    }

    if(i!=points->length())
    {
        times<<points->at(i);
        strRes=meter->trig();
        list=  UserfulFunctions::resultPro(strRes);
        item1 << list.at(0);
        item2 << list.at(1);
        updatePlot();
    }

    GoOut:



    emit showProgress(100);

}

void clsTimeMeasument::stop()
{
    isStop = true;
    qApp->processEvents();
}

void clsTimeMeasument::setPlot(Plot *plot)
{
    this->plot=plot;
}

void clsTimeMeasument::setXLog(bool /*value*/)
{

}

void clsTimeMeasument::setMeter(WKEInstrument *meter)
{
    this->meter = meter;
}

void clsTimeMeasument::setGraphSettings(graphSetup /*gs*/)
{

}

void clsTimeMeasument::updatePlot()
{
    plot->showData(times,item1,item2);
}

void clsTimeMeasument::singleShot()
{

}

void clsTimeMeasument::setTimer()
{
    timer.setInterval(1000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updatePlot()));
    timer.start();
}

double clsTimeMeasument::average(const QVector<double> &tmp)
{
    double sum=0;

    if(tmp.size()==0)
        return 0.0;

    for(int i=0; i<tmp.size(); i++)
    {
        sum+=tmp.at(i);
    }
    return sum/tmp.size();
}
