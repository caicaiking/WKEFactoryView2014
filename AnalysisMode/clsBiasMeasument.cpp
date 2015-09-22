#include "clsBiasMeasument.h"
#include "UserfulFunctions.h"
#include "clsRuningSettings.h"
void clsBiasMeasument::setMin(double value)
{
    this->dblMin=value;
}

void clsBiasMeasument::setMax(double value)
{
    this->dblMax=value;
}

void clsBiasMeasument::setItems(const QString &item1, const QString &item2)
{
    this->testItem1=item1;
    this->testItem2=item2;

}

void clsBiasMeasument::setPoint(QList<double> *point)
{
    this->points = point;

      qSort(*this->points);
}

void clsBiasMeasument::setItemsAndPoints(const QString &item1, const QString &item2,
                                         QList<double>  *point)
{
    setItems(item1,item2);
    setPoint(point);
}

void clsBiasMeasument::trig()
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

        meter->setBias(tmp,"V");
        points->removeAt(i);
        double tmpBias;
        QString tmpUnit;
        meter->getBias(&tmpBias,&tmpUnit);
        meter->turnOnBias();
        points->insert(i,tmpBias);
        QString strRes=meter->trig();

        QList<double> res=UserfulFunctions::resultPro(strRes);

        bias<<tmpBias;
        item1<<res.at(0);
        item2<<res.at(1);
        updatePlot();
        qApp->processEvents();
        emit showProgress((int)((i+1)*100/points->length()));
        qApp->processEvents();

       // qDebug()<<"Bias Rate: "<<gs.biasVRate;
        if((i<points->length()-1) && (gs.biasVRate!=0))
        {
            double disVoltage = points->at(i+1)-points->at(i);
            int delayTime =disVoltage/gs.biasVRate*1000.0;
           // qDebug()<<"Delay Time" << delayTime;
            UserfulFunctions::sleepMs(delayTime);
        }

    }
STOP:
    meter->turnOffBias();
    emit  showProgress(100);
}

void clsBiasMeasument::stop()
{
    isStop = true;
    qApp->processEvents();
}

void clsBiasMeasument::setPlot(Plot *plot)
{
    this->plot = plot;
}

void clsBiasMeasument::setXLog(bool value)
{
    this->xLog = value;
}

void clsBiasMeasument::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}

void clsBiasMeasument::setGraphSettings(graphSetup gs)
{
    this->gs=gs;
}

void clsBiasMeasument::updatePlot()
{
    plot->showData(bias,item1,item2);
}

