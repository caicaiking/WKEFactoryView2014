#include "clsFreqMeasument.h"
#include "Globle.h"
#include "UserfulFunctions.h"
#include "clsRuningSettings.h"
#include <QDebug>
#include "clsSettings.h"
#include <QListIterator>
void clsFreqMeasument::setMin(double value)
{
    this->dblMin=value;
}

void clsFreqMeasument::setMax(double value)
{
    this->dblMax=value;
}

void clsFreqMeasument::setItems(const QString &item1, const QString &item2)
{
    this->testItem1=item1;
    this->testItem2=item2;

}

void clsFreqMeasument::setPoint(QList<double> *point)
{
    this->points = point;
}

void clsFreqMeasument::setItemsAndPoints(const QString &item1, const QString &item2,
                                         QList<double> *point)
{
    setItems(item1,item2);
    setPoint(point);
}

void clsFreqMeasument::trig()
{

    isStop= false;
    freq.clear();
    item1.clear();
    item2.clear();

    for(int i=0;i<points->length();i++)
    {
        if(isStop)
            goto STOP;
        double tmp = points->at(i);

        meter->setFrequency(tmp);
        points->removeAt(i);
        double tmpFreq;
        meter->getFrequency(&tmpFreq);
       // qDebug()<< "tmp Freq: "<<tmpFreq;
        points->insert(i,tmpFreq);
        QString strRes=meter->trig();
        QList<double> res=UserfulFunctions::resultPro(strRes);
        freq<<tmpFreq;
        item1<<res.at(0);
        item2<<res.at(1);
        updatePlot();
        qApp->processEvents();
        emit showProgress((int)((i+1)*100/points->length()));
        qApp->processEvents();
    }
STOP:
    emit  showProgress(100);
}

void clsFreqMeasument::stop()
{
    isStop = true;
    qApp->processEvents();
}

void clsFreqMeasument::setPlot(Plot *plot)
{
    this->plot = plot;
}

void clsFreqMeasument::setXLog(bool value)
{
    this->xLog = value;
}

void clsFreqMeasument::setMeter(WKEInstrument *meter)
{
    this->meter = meter;
}

void clsFreqMeasument::setGraphSettings(graphSetup /*gs*/)
{

}

void clsFreqMeasument::updatePlot()
{
    plot->showData(freq,item1,item2);

}

