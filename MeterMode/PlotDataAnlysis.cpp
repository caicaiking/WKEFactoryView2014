#include "PlotDataAnlysis.h"
#include <QDebug>
#include <cmath>
PlotDataAnlysis::PlotDataAnlysis()
{

}

void PlotDataAnlysis::setTestData(const QVector<double> &testData)
{
    this->testData = testData;
    bars =  getBarStatic();
    fitData = getNormCurve();
}

QVector<SINGLE_BAR> PlotDataAnlysis::getBars()
{
    return this->bars;
}

QVector<QPointF> PlotDataAnlysis::getFitData()
{
    return this->fitData;
}

QVector<SINGLE_BAR> PlotDataAnlysis::getBarStatic()
{
    QVector<SINGLE_BAR> tmp;
    if(testData.length()<=0)
        return tmp;

    qSort(testData);

    PlotSettings ps;
    ps.minX = testData.first();
    ps.maxX = testData.last();

    ps.adjust();

    int barCount = ps.numXTicks.length() + (int)(testData.length() /100);

    if(barCount>=40)
        barCount = 40;
    if(barCount<7)
        barCount=7;

    double step = (testData.last()- testData.first())/barCount;

    for(int i=1; i<= barCount; i++)
    {
        SINGLE_BAR sb ;
        sb.barValue=0;
        sb.x=(testData.first()+step*(i-1));
        sb.y=(testData.first()+step*(i));
        tmp.append(sb);
    }
    tmp[tmp.length()-1].y = testData.last();
    for(int i=0; i< testData.length();i++)
    {
        for(int j=0; j< tmp.length(); j++)
        {
            if(testData.at(i) <= tmp.at(j).y && testData.at(i)>= tmp.at(j).x)
                tmp[j].barValue+=1;
        }
    }

    this->pltSettings.minX = testData.first();
    this->pltSettings.maxX = testData.last();

    pltSettings.minX = pltSettings.minX ;
    pltSettings.maxX = pltSettings.maxX;
    pltSettings.adjust();

    int max = tmp.at(0).barValue;
    for(int i=1; i< tmp.length();i++)
    {
        if(max< tmp.at(i).barValue)
            max = tmp.at(i).barValue;
    }
    // qDebug()<< max;
    pltSettings.maxY = max*1.1;
    pltSettings.adjust();



    return tmp;
}

QVector<QPointF> PlotDataAnlysis::getNormCurve()
{
    QVector<QPointF> tmp;
    if(testData.length() <=2)
        return tmp;

    double sumData=0;
    for(int i=0; i< testData.length(); i++)
    {
        sumData+= testData.at(i);
    }

    meanValue = sumData / testData.length();

    sumData=0;

    for(int i=0; i< testData.length(); i++)
    {
        sumData += pow(testData.at(i)- meanValue,2.0);
    }

    stdValue = sqrt(sumData/(testData.length()-1));

    neg3Std = meanValue - stdValue*3;
    pos3Std = meanValue + stdValue*3;

    double startX= neg3Std;
    if(startX< pltSettings.minX)
        startX = pltSettings.minX;

    double stopX = pos3Std;
    if(stopX> pltSettings.maxX)
        stopX= pltSettings.maxX;

    //获取绘图的数据x y
    double step = (pltSettings.maxX - pltSettings.minX)/getBars().length();

    for(int i=0; i< 50; i++)
    {
        QPointF tmpPoint;
        double x, y;
        x = startX + (stopX-startX)/50.0 *i;
        double temp;
        temp = ( x - meanValue) / stdValue;
        y = exp(-0.5 * temp *temp )/ (sqrt(2 * 3.14159) * stdValue);
        y = y* testData.length() * step;

        tmpPoint.setX(x);
        tmpPoint.setY(y);

        tmp.append(tmpPoint);
    }

    //调整坐标系最大值
    double max=0;
    for(int i=0; i< tmp.length(); i++)
    {
        if(max < tmp.at(i).y())
            max = tmp.at(i).y();
    }

    if(pltSettings.maxY <= max)
        pltSettings.maxY = max *1.05;
    return tmp;
}

double PlotDataAnlysis::getPos3Std() const
{
    return pos3Std;
}

double PlotDataAnlysis::getNeg3Std() const
{
    return neg3Std;
}

double PlotDataAnlysis::getStdValue() const
{
    return stdValue;
}

double PlotDataAnlysis::getMeanValue() const
{
    return meanValue;
}

PlotSettings PlotDataAnlysis::getSettings() const
{
    return this->pltSettings;
}

