#include "clsBiasAMeasument.h"
#include "UserfulFunctions.h"
#include "clsRuningSettings.h"
void clsBiasAMeasument::setMin(double value)
{
    this->dblMin=value;
}

void clsBiasAMeasument::setMax(double value)
{
    this->dblMax=value;
}

void clsBiasAMeasument::setItems(const QString &item1, const QString &item2)
{
    this->testItem1=item1;
    this->testItem2=item2;

}

void clsBiasAMeasument::setPoint(QList<double> *point)
{
    this->points = point;
}

void clsBiasAMeasument::setItemsAndPoints(const QString &item1, const QString &item2,
                                          QList<double>  *point)
{
    setItems(item1,item2);
    setPoint(point);
}

void clsBiasAMeasument::trig()
{

    isStop= false;
    bias.clear();
    item1.clear();
    item2.clear();


    for(int i=0;i<points->length();i++)
    {
        if(isStop)
            goto STOP;
        double tmp = points->at(i);

        meter->setBias(tmp,"A");
        points->removeAt(i);
        double tmpBias;
        QString tmpUnit;
        meter->getBias(&tmpBias,&tmpUnit);
        meter->turnOnBias();
        points->insert(i,tmpBias);
        QString strRes=meter->trig();

        QList<double> res=UserfulFunctions::resultPro(strRes);

        bias<<tmp;
        item1<<res.at(0);
        item2<<res.at(1);
        updatePlot();
        qApp->processEvents();
        emit showProgress((int)((i+1)*100/points->length()));
        qApp->processEvents();
    }
STOP:
    meter->turnOffBias();
    emit  showProgress(100);
}

void clsBiasAMeasument::stop()
{
    isStop = true;
    qApp->processEvents();
}

void clsBiasAMeasument::setPlot(Plot *plot)
{
    this->plot = plot;
}

void clsBiasAMeasument::setXLog(bool value)
{
    this->xLog = value;
}

void clsBiasAMeasument::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}

void clsBiasAMeasument::setGraphSettings(graphSetup /*gs*/)
{

}

void clsBiasAMeasument::updatePlot()
{
    plot->showData(bias,item1,item2);
}

