#include "multiModePlot.h"
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_engine.h>
#include "UserfulFunctions.h"
multiModePlot::multiModePlot(QWidget *parent) : QwtPlot(parent)
{
    this->setAutoReplot(false);
    //setGraphTitle("Unititled");
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius(5 );
    setCanvas( canvas );
    setCanvasBackground(QColor(Qt::black));
    //this->setStyleSheet("background-color: black;");
    //y axis Left grid
    QwtPlotGrid*   gridYLeft=new QwtPlotGrid;
    gridYLeft->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    gridYLeft->setMajorPen(QPen(QColor(0,128,128),0,Qt::DotLine));
    gridYLeft->setMinorPen(QPen(QColor(0,64,64),0,Qt::DotLine));
    gridYLeft->enableYMin(true);
    gridYLeft->enableX(false);
    gridYLeft->attach(this);

    //x axis grid
    QwtPlotGrid*    gridX=new QwtPlotGrid;
    gridX->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    gridX->enableY(false);
    gridX->enableYMin(false);
    gridX->enableXMin(true);
    gridX->setMajorPen(QPen(QColor(0,192,0),0,Qt::DotLine));
    gridX->setMinorPen(QPen(QColor(0,96,0),0,Qt::DotLine));
    gridX->attach(this);
    this->enableAxis(QwtPlot::yLeft,true);
    this->setAxisMaxMajor(QwtPlot::xBottom,6);
    this->setAxisMaxMajor(QwtPlot::yLeft,10);
    this->setAxisScaleEngine(QwtPlot::xBottom,
                             new QwtLinearScaleEngine);
    this->setAxisScaleEngine(QwtPlot::yLeft,
                             new QwtLinearScaleEngine);
    this->setAxisScale(QwtPlot::xBottom,0,10);
    this->setAxisScale(QwtPlot::yLeft,1,10);
    this->setAxisFont(QwtPlot::yLeft,QFont("Times", 10));
    this->setAxisFont(QwtPlot::xBottom,QFont("Times", 10));
    this->setAutoReplot(true);

    setXTrace(QString(tr("次数")));
}

void multiModePlot::setChannalAndItems(QString channel, QStringList items)
{
    QList<int> keys = testData.keys();
    for(int i=0; i< keys.length(); i++)
    {
        testData[keys.at(i)].setVisable(false);
    }

    testData.clear();
    QStringList color;
//    color<<"#e0861a"<<"#99cc00"<<"#9f991a"<<"#a48033"<<"#a9664d"<<"#ae4d66"<<"#b33380"<<"#dbce8f"
//        <<"#a64086"<<"#994d8d"<<"#8d5a93"<<"#806699"<<"#8073a6"<<"#8080b3"<<"#ffd400"
//       <<"#145b7d"<<"#99b389"<<"#11264f"<<"#99e600"<<"#8a2e3b";
    color <<"#84DE02"<<"#FFBF00"<<"#FF033E"<<"#00FFFF"<<"#D0FF14"<<"#FDEE00"<<"#FF2052"<<"#E0218A"
         <<"#FA6E79"<<"#FE6F5E"<<"#ACE5EE"<<"#1F75FE"<<"#ACE5EE"<<"#66FF00"<<"#D891EF"<<"#FF007F"
        <<"#08E8DE"<<"#D19FE8"<<"#FFAA1D"<<"#FF55A3";

    for(int i=0; i< channel.split(",").length(); i++)
    {
        QString text = channel.split(",").at(i);
        if(!text.isEmpty())
        {
            TestCurve  cur;

            QwtPlotCurve* curve =new QwtPlotCurve("curve");
//            curve->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
//                                           QPen(Qt::red), QSize(5, 5) ) );

            cur.setCurve(curve);
            cur.getCurve()->attach(this);
            cur.setColor(color.at(i%20));
            cur.clearData();
            cur.clearPoint();

            for(int j=0; j< items.length(); j++)
            {
                QVector<double> tmpx;
                cur.initData(j,tmpx);
            }

            testData.insert(text.toInt(), cur);
        }
    }

}

void multiModePlot::setCurveVisable(QList<bool> value)
{
    // qDebug()<< "value length: "<<value.length() << " " << "TestData: "<<testData.count();

    if(testData.count()!= value.length())
        return;


    for(int i=0; i< testData.keys().length(); i++)
    {
        int key = testData.keys().at(i);
        testData[key].setVisable(value.at(i));
    }
}


void multiModePlot::clearData()
{
    for(int i=0; i< testData.keys().length(); i++)
    {
        int key = testData.keys().at(i);
        testData[key].clearData(i);
        testData[key].clearPoint();
    }
}

void multiModePlot::setCurrentIndex(int index)
{
   // qDebug()<<"TestData Length: "<<testData.count();


    for(int i=0; i< testData.keys().length(); i++)
    {
        int key = testData.keys().at(i);
        testData[key].setDataIndex(index);
    }

    this->replot();
}

void multiModePlot::setXTrace(const QString &value)
{

    QwtText tmp;
    tmp.setFont(QFont("楷体",12,QFont::Bold));
    tmp.setText(QString("%1(%2)").arg(value).arg(UserfulFunctions::getSuffix(value)));

    tmp.setColor(QColor(0,120,0));

    this->setAxisTitle(QwtPlot::xBottom,tmp);
}

void multiModePlot::setData(int channal, double point, QList<double> data)
{
    testData[channal].setData(point,data);
    this->setAxisAutoScale(QwtPlot::yLeft,true);
    this->setAxisAutoScale(QwtPlot::xBottom,true);
}

void multiModePlot::setGraphTitle(const QString & strTitle)
{
    //QString title =QString("<p> <font size=\"3\" font face=\"楷体\" color=\"black\">%1</p>").arg(strTitle);
    QwtText txt;
    txt.setText(strTitle);
    txt.setFont(QFont("楷体",12,QFont::Bold));

    this->setTitle(txt);
}
