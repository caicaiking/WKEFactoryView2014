#include "clsBiasAMeasument.h"
#include "UserfulFunctions.h"
#include "clsRuningSettings.h"
#include "clsBiasAOp.h"
#include "clsSampleTest.h"
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

    qSort(*this->points);
}

void clsBiasAMeasument::setItemsAndPoints(const QString &item1, const QString &item2,
                                          QList<double>  *point)
{
    setItems(item1,item2);
    setPoint(point);
}

void clsBiasAMeasument::trig()
{
    clsSampleTest *dlg = new clsSampleTest(meter,0);
    if(dlg->exec() == QDialog::Rejected)
    {
        return;
    }

    isStop= false;
    bias.clear();
    item1.clear();
    item2.clear();

    sngBiasAOp::Instance()->setMeter(this->meter);
    sngBiasAOp::Instance()->readSettings();


    for(int i=0;i<points->length();i++)
    {
        double tmpBias=0;
        QString tmpUnit="";

        if(isStop)
            goto STOP;
        double tmp = points->at(i);

        meter->setBias(tmp,"A");
        points->removeAt(i);
        if(tmp!=0)
            meter->getBias(&tmpBias,&tmpUnit);
        points->insert(i,tmpBias);
        if(!bias.contains(tmpBias))
        {
            if(tmpBias!=0)
            {
                if(!meter->turnOnBias())
                    goto NextBiasPoint;
                sngBiasAOp::Instance()->preOperation();
            }
            QString strRes=meter->trig();
            QList<double> res=UserfulFunctions::resultPro(strRes);
            bias<<tmpBias;
            item1<<res.at(0);
            item2<<res.at(1);
            updatePlot();
            if(i<points->length()-1)
                sngBiasAOp::Instance()->afterOperation();
        }
NextBiasPoint:
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

