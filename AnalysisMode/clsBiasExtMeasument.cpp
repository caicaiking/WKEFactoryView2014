#include "clsBiasExtMeasument.h"

#include "UserfulFunctions.h"
#include "clsRuningSettings.h"
#include "clsEaPs8000.h"
clsBiasExtMeasument::clsBiasExtMeasument()
{



}

clsBiasExtMeasument::~clsBiasExtMeasument()
{
    //power->turnOFF();
}

void clsBiasExtMeasument::setMin(double value)
{
    this->dblMin=value;
}

void clsBiasExtMeasument::setMax(double value)
{
    this->dblMax=value;
}

void clsBiasExtMeasument::setItems(const QString &item1, const QString &item2)
{
    this->testItem1=item1;
    this->testItem2=item2;

}

void clsBiasExtMeasument::setPoint(QList<double> *point)
{
    this->points = point;
    qSort(*this->points);
}

void clsBiasExtMeasument::setItemsAndPoints(const QString &item1, const QString &item2,
                                            QList<double>  *point)
{
    setItems(item1,item2);
    setPoint(point);
}

void clsBiasExtMeasument::trig()
{
    power = new clsEaPs8000();

    if(!power->init())
        qWarning()<<"No power supply connect!";

    isStop= false;
    bias.clear();
    item1.clear();
    item2.clear();

    power->turnON();
    power->setVoltage(0);


    for(int i=0;i<points->length();i++)
    {
        if(isStop)
        {
            power->turnOFF();
            power->disConnect();
            goto STOP;
        }
        double tmp = points->at(i);

        power->setVoltage(tmp);
        points->removeAt(i);
        //double tmpBias = power->getVoltage();
        points->insert(i,tmp); //准确做法

        UserfulFunctions::sleepMs(1000);
        QString strRes=meter->trig();

        QList<double> res=UserfulFunctions::resultPro(strRes);

        bias<<tmp;//非精确做法
        item1<<res.at(0);
        item2<<res.at(1);
        updatePlot();
        qApp->processEvents();
        emit showProgress((int)((i+1)*100/points->length()));
        qApp->processEvents();
    }
STOP:
    power->setVoltage(0.0);
    power->turnOFF();
    power->disConnect();

    delete power;

    emit  showProgress(100);
}

void clsBiasExtMeasument::stop()
{
    isStop = true;
    qApp->processEvents();
}

void clsBiasExtMeasument::setPlot(Plot *plot)
{
    this->plot = plot;
}

void clsBiasExtMeasument::setXLog(bool value)
{
    this->xLog = value;
}

void clsBiasExtMeasument::setMeter(WKEInstrument *meter)
{
    this->meter=meter;
}

void clsBiasExtMeasument::setGraphSettings(graphSetup gs)
{
    this->gs=gs;
}

void clsBiasExtMeasument::updatePlot()
{
    plot->showData(bias,item1,item2);
}
