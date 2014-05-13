#include "Plot.h"
#include <Qwt/qwt_plot_marker.h>
#include <Qwt/qwt_plot_curve.h>
#include <Qwt/qwt_legend.h>
#include <Qwt/qwt_text.h>
#include <Qwt/qwt_plot_grid.h>
#include <Qwt/qwt_symbol.h>
#include <Qwt/qwt_scale_engine.h>
#include <QMouseEvent>
#include <UserfulFunctions.h>
#include <doubleType.h>
#include "clsRefTraceProperty.h"
Plot::Plot(QWidget *parent) :
    QwtPlot(parent)
{
    this->setAutoReplot(false);
    setGraphTitle("Unititled");
    this->setCanvasBackground(QColor(Qt::black));

    //this->setStyleSheet("background-color: black;");
    blLogX=true;
    blLogYLeft=true;


    //y axis Left grid
    gridYLeft=new QwtPlotGrid;
    gridYLeft->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    gridYLeft->setMajPen(QPen(QColor(0,128,128),0,Qt::DotLine));
    gridYLeft->setMinPen(QPen(QColor(0,64,64),0,Qt::DotLine));
    gridYLeft->enableYMin(true);
    gridYLeft->enableX(false);
    gridYLeft->attach(this);

    //y axis Right grid
    gridYRight=new QwtPlotGrid;
    gridYRight->setAxes(QwtPlot::xBottom,QwtPlot::yRight);
    gridYRight->setMajPen(QPen(QColor(96,96,0),0,Qt::DotLine));
    gridYRight->setMinPen(QPen(QColor(96,96,0),0,Qt::DotLine));
    gridYRight->enableYMin(false);
    gridYRight->enableX(false);
    gridYRight->attach(this);

    //x axis grid
    gridX=new QwtPlotGrid;
    gridX->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    gridX->enableY(false);
    gridX->enableYMin(false);
    gridX->enableXMin(true);
    gridX->setMajPen(QPen(QColor(0,192,0),0,Qt::DotLine));
    gridX->setMinPen(QPen(QColor(0,96,0),0,Qt::DotLine));
    gridX->attach(this);



    this->enableAxis(QwtPlot::yLeft,true);
    this->enableAxis(QwtPlot::yRight);
    setTraces("Z",QString("A"),tr("频率"));
    this->setAxisMaxMajor(QwtPlot::xBottom,6);
    this->setAxisMaxMajor(QwtPlot::yLeft,10);
    if(blLogX)
        this->setAxisScaleEngine(QwtPlot::xBottom,
                                 new QwtLog10ScaleEngine);
    else
        this->setAxisScaleEngine(QwtPlot::xBottom,
                                 new QwtLinearScaleEngine);
    if(blLogYLeft)
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLog10ScaleEngine );
    else
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLinearScaleEngine);

    this->setAxisScale(QwtPlot::xBottom,10,1000);
    this->setAxisScale(QwtPlot::yLeft,1,1000);
    this->setAxisScale(QwtPlot::yRight,1,10);

    this->setAxisFont(QwtPlot::yLeft,QFont("Times", 10));
    this->setAxisFont(QwtPlot::yRight,QFont("Times", 10));
    this->setAxisFont(QwtPlot::xBottom,QFont("Times", 10));

    d_marker1 = new QwtPlotMarker;
    d_marker1->setValue(100000,0.0);
    d_marker1->setLineStyle(QwtPlotMarker::VLine);
    d_marker1->setLabelAlignment(Qt::AlignRight |
                                 Qt::AlignBottom);
    d_marker1->setLinePen(QPen(Qt::red,0,Qt::SolidLine));
    d_marker1->attach(this);

    this->setAutoReplot(true);
    setTraceA(QString("Z"));
    setTraceB(QString("A"));
    setXTrace(QString(tr("频率")));
}


void Plot::mouseMoveEvent(QMouseEvent *e)
{
//    if(e->buttons() & Qt::LeftButton)
//    {
//        double freq=this->invTransform(QwtPlot::xBottom, e->x());
//        qDebug() << "Move frequency is: "<< freq/1000.0;
//    }

//    else
//        qDebug()<<"No thing can be user";



    // d_marker1->setValue();

}

void Plot::clearData()
{
    QVector<double> x;
    QVector<double> curvel;
    QVector<double> curve2;
    if(curves.count() <1)
        return;

    if(curves.contains(0))
    {
        curves.value(0).cur1->setSamples(x,curvel);
        curves.value(0).cur2->setSamples(x,curve2);
    }

}

void Plot::showData(const QVector<double>& x,
                    const QVector<double> &curvel,
                    const QVector<double> &curve2)
{
    if(curves.count() <1)
        return;

    if(curves.contains(0))
    {
        curves.value(0).cur1->setSamples(x,curvel);
        curves.value(0).cur2->setSamples(x,curve2);


    }
    //在这儿不自动缩放坐标系
    // this->setAxisScale(QwtPlot::xBottom,x.first(),x.last());

}

void Plot::autoScale(Plot::Choice choice)
{

    switch(choice)
    {
    case yLeft:
        this->setAxisAutoScale(QwtPlot::yLeft,true);
        break;
    case yRight:
        this->setAxisAutoScale(QwtPlot::yRight,true);
        break;
    case Both:
        this->setAxisAutoScale(QwtPlot::yLeft,true);
        this->setAxisAutoScale(QwtPlot::yRight,true);
        break;
    }

    this->setAxisAutoScale(QwtPlot::yLeft,false);
    this->setAxisAutoScale(QwtPlot::yRight,false);

}

void Plot::setGraphTitle(const QString & strTitle)
{
    //QString title =QString("<p> <font size=\"3\" font face=\"楷体\" color=\"black\">%1</p>").arg(strTitle);
    QwtText txt;
    txt.setText(strTitle);
    txt.setFont(QFont("楷体",12,QFont::Bold));

    this->setTitle(txt);
}

void Plot::addNewCurve(const int index,
                       const QColor curColor1,
                       const QString curTitle1,
                       const QString curColor2,
                       const bool OnOrOff)
{

    QwtPlotCurve *d_curve1;


    d_curve1 = new QwtPlotCurve(curTitle1);
    d_curve1->setRenderHint(QwtPlotItem::RenderAntialiased);
    d_curve1->setPen(QPen(curColor1));
    d_curve1->setLegendAttribute(QwtPlotCurve::LegendShowLine);
    d_curve1->setYAxis(QwtPlot::yLeft);

    QwtPlotCurve *d_curve2;
    d_curve2 = new QwtPlotCurve(curTitle1);
    d_curve2->setRenderHint(QwtPlotItem::RenderAntialiased);
    d_curve2->setPen(QPen(curColor2));
    d_curve2->setLegendAttribute(QwtPlotCurve::LegendShowLine);
    d_curve2->setYAxis(QwtPlot::yRight);



    PlotCurves cr;
    cr.cur1=d_curve1;
    cr.cur2=d_curve2;
    curves.insert(index,cr);

    if(OnOrOff)
    {
        curves.value(index).cur1->attach(this);
        curves.value(index).cur2->attach(this);
    }
    else
    {
        curves.value(index).cur1->detach();
        curves.value(index).cur2->detach();
    }
}

void Plot::addNewCurve(curveProperty property, bool isSetCurrent)
{
   // qDebug()<<"Ref index: " << property.index;

    if(curves.contains(property.index))
    {
        //qDebug()<<"Ref index: " << property.index<<"  user old curves";
        curves.value(property.index).cur1->setPen(QPen(property.traceA));
        curves.value(property.index).cur2->setPen(QPen(property.traceB));
        curves.value(property.index).cur1->setTitle(property.title);
        curves.value(property.index).cur2->setTitle(property.title);
    }
    else
    {
      //  qDebug()<<"Ref index: " << property.index<<"  creat new curves";
        QwtPlotCurve *d_curve1;


        d_curve1 = new QwtPlotCurve(property.title);
        d_curve1->setRenderHint(QwtPlotItem::RenderAntialiased);
        d_curve1->setPen(QPen(property.traceA));
        d_curve1->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        d_curve1->setYAxis(QwtPlot::yLeft);

        QwtPlotCurve *d_curve2;
        d_curve2 = new QwtPlotCurve(property.title);
        d_curve2->setRenderHint(QwtPlotItem::RenderAntialiased);
        d_curve2->setPen(QPen(property.traceB));
        d_curve2->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        d_curve2->setYAxis(QwtPlot::yRight);

        PlotCurves cr;
        cr.cur1=d_curve1;
        cr.cur2=d_curve2;
        curves.insert(property.index,cr);
    }

    if(isSetCurrent)
    {
        curves.value(property.index).cur1->setSamples(UserfulFunctions::formatToData(curves.value(0).cur1).toVector());
        curves.value(property.index).cur2->setSamples(UserfulFunctions::formatToData(curves.value(0).cur2).toVector());
    }

    if(property.isOn)
    {
        if(this->axisEnabled(QwtPlot::yLeft))
            curves.value(property.index).cur1->attach(this);
        else
            curves.value(property.index).cur1->detach();

        if(this->axisEnabled(QwtPlot::yRight))
            curves.value(property.index).cur2->attach(this);
        else
            curves.value(property.index).cur2->detach();
    }
    else
    {
        curves.value(property.index).cur1->detach();
        curves.value(property.index).cur2->detach();
    }

    this->replot();
}

void Plot::setTraceA(const QString &value)
{
    this->strTraceA=value;
    QwtText tmp;
    tmp.setFont(QFont("楷体",12,QFont::Bold));
    tmp.setText(QString("%1(%2)").arg(this->strTraceA).arg(UserfulFunctions::getSuffix(strTraceA)));
    tmp.setColor(Qt::cyan);
    this->setAxisTitle(QwtPlot::yLeft,
                       tmp);

    //qDebug()<<"Trace A" <<strTraceA;

    QString logValue("ZY");
    if(!logValue.contains(strTraceA))
    {
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLinearScaleEngine);
    }


}

void Plot::setTraceB(const QString &value)
{
    this->strTraceB=value;
    QwtText tmp;
    tmp.setFont(QFont("楷体",12,QFont::Bold));
    tmp.setText(QString("%1(%2)").arg(this->strTraceB).arg(UserfulFunctions::getSuffix(strTraceB)));
    tmp.setColor(QColor::fromRgb(255,215,0));
    this->setAxisTitle(QwtPlot::yRight,tmp);
}

void Plot::setXTrace(const QString &value)
{
    this->strXTrace=value;
    QwtText tmp;
    tmp.setFont(QFont("楷体",12,QFont::Bold));
    tmp.setText(QString("%1(%2)").arg(strXTrace).arg(UserfulFunctions::getSuffix(strXTrace)));

    tmp.setColor(QColor(0,120,0));

    this->setAxisTitle(QwtPlot::xBottom,tmp);
}

void Plot::setTraces(const QString &traceA, const QString &traceB, const QString &xTrace)
{
    setTraceA(traceA);
    setTraceB(traceB);
    setXTrace(xTrace);
}

void Plot::setXScale(double min, double max, bool logx)
{
    this->setAxisScale(QwtPlot::xBottom,min,max);
    this->xmax=max;
    this->xmin=min;
    setBlLogX(logx);
}

void Plot::setYLeftScal(double min, double max, bool logx)
{
    this->setAxisScale(QwtPlot::yLeft,min,max);
    this->yLeftMin =min;
    this->yLeftMax=max;
    setBlLogYLeft(logx);
}

void Plot::setYRightScal(double min, double max)
{
    this->yRightMax=max;
    this->yRightMin =min;
    this->setAxisScale(QwtPlot::yRight,min,max);
}

void Plot::setScale(double xmin, double xmax, bool blLogX,
                    double yLeftMin, double yLeftMax, bool blLogYLeft,
                    double yRightMin, double yRightMax)
{
    setXScale(xmin,xmax,blLogX);
    setYLeftScal(yLeftMin,yLeftMax,blLogYLeft);
    setYRightScal(yRightMin,yRightMax);
}

void Plot::getScale(double &xmin,  double &xmax, bool &blLogX,
                    double &yLeftMin, double &yLeftMax, bool &blLogYLeft,
                    double &yRightMin, double &yRightMax)
{
    xmin=getXmin();
    xmax=getXmax();
    blLogX =this->getBlLogX();

    yLeftMin=this->getYLeftMin();
    yLeftMax=this->getYLeftMax();
    blLogYLeft=this->getBlLogYLeft();

    yRightMin =this->getYRightMin();
    yRightMax =this->getYRightMax();
}


void Plot::turnOffCurves( QList<curveProperty> cp,
                          Plot::Choice choice, bool status)
{

    switch(choice)
    {
    case yLeft:
        this->enableAxis(QwtPlot::yLeft,status);
        if(status)
            gridYLeft->attach(this);
        else
            gridYLeft->detach();
        break;
    case yRight:
        this->enableAxis(QwtPlot::yRight,status);
        if(status)
            gridYRight->attach(this);
        else
            gridYRight->detach();
        break;
    case Both:
        this->enableAxis(QwtPlot::yLeft,status);
        this->enableAxis(QwtPlot::yRight,status);
        if(status)
        {
            gridYLeft->attach(this);
            gridYRight->attach(this);
        }
        else
        {
            gridYLeft->detach();
            gridYRight->detach();
        }
        break;
    }

    QMapIterator <int,PlotCurves>it(curves);
    while(it.hasNext())
    {
        it.next();
        if(status && cp.at(it.key()).isOn)
        {
            switch (choice)
            {
            case yLeft:
                it.value().cur1->attach(this);
                //                gridYLeft->attach(this);
                //                this->enableAxis(QwtPlot::yLeft,true);
                break;
            case yRight:
                it.value().cur2->attach(this);
                //                gridYRight->attach(this);
                //                this->enableAxis(QwtPlot::yRight,true);
                break;
            case Both:
                it.value().cur1->attach(this);
                it.value().cur2->attach(this);
                //                gridYLeft->attach(this);
                //                gridYRight->attach(this);
                //                this->enableAxis(QwtPlot::yLeft,true);
                //                this->enableAxis(QwtPlot::yRight,true);
            }
        }
        else
        {
            switch (choice)
            {
            case yLeft:
                it.value().cur1->detach();
                //                gridYLeft->detach();
                //                this->enableAxis(QwtPlot::yLeft,false);
                break;
            case yRight:
                it.value().cur2->detach();
                //                gridYRight->detach();
                //                this->enableAxis(QwtPlot::yRight,false);
                break;
            case Both:
                it.value().cur1->detach();
                it.value().cur2->detach();
                //                gridYLeft->detach();
                //                gridYRight->detach();
                //                this->enableAxis(QwtPlot::yLeft,false);
                //                this->enableAxis(QwtPlot::yRight,false);
            }
        }
    }
    this->replot();
    //将Marker更新一次
    setMarker(dblMarkValue);
}

void Plot::turnOffCurves(Plot::Choice choice, bool status)
{

    QMapIterator <int,PlotCurves>it(curves);
    while(it.hasNext())
    {
        it.next();
        if(status)
        {
            switch (choice)
            {
            case yLeft:
                it.value().cur1->attach(this);
                gridYLeft->attach(this);
                this->enableAxis(QwtPlot::yLeft,true);
                break;
            case yRight:
                it.value().cur2->attach(this);
                gridYRight->attach(this);
                this->enableAxis(QwtPlot::yRight,true);
                break;
            case Both:
                it.value().cur1->attach(this);
                it.value().cur2->attach(this);
                gridYLeft->attach(this);
                gridYRight->attach(this);
                this->enableAxis(QwtPlot::yLeft,true);
                this->enableAxis(QwtPlot::yRight,true);
            }
        }
        else
        {
            switch (choice)
            {
            case yLeft:
                it.value().cur1->detach();
                gridYLeft->detach();
                this->enableAxis(QwtPlot::yLeft,false);
                break;
            case yRight:
                it.value().cur2->detach();
                gridYRight->detach();
                this->enableAxis(QwtPlot::yRight,false);
                break;
            case Both:
                it.value().cur1->detach();
                it.value().cur2->detach();
                gridYLeft->detach();
                gridYRight->detach();
                this->enableAxis(QwtPlot::yLeft,false);
                this->enableAxis(QwtPlot::yRight,false);
            }
        }
    }
    this->replot();
    //将Marker更新一次
    setMarker(dblMarkValue);
}

QString Plot::setMarker(const double &freq, const int /*intSelected*/)
{
    QMap<double,QPointF> myPrivateData;
    myPrivateData = UserfulFunctions::formatToData(curves.values(),0);

    QList<double> keys= myPrivateData.keys();
    //找到数组中的频点，然后在获取相关的数值
    double freqValue, item1, item2;
    if(freq>UserfulFunctions::Max(keys))
        freqValue= UserfulFunctions::Max(keys);
    else if(freq<UserfulFunctions::Min(keys))
        freqValue= UserfulFunctions::Min(keys);
    else
        freqValue= UserfulFunctions::NearItem(keys,freq,blLogYLeft);

    dblMarkValue=freqValue;
    item1=((QPointF) myPrivateData.value(freqValue)).x();
    item2=((QPointF) myPrivateData.value(freqValue)).y();
    d_marker1->setValue(freqValue,0.0);

    //格式化输出
    doubleType dt;
    dt.setData(freqValue,"");
    QString strX= dt.formateToString() + UserfulFunctions::getSuffix(strXTrace);
    dt.setData(item1,"");
    QString strItem1= dt.formateToString() + UserfulFunctions::getSuffix(strTraceA);
    dt.setData(item2,"");
    QString strItem2=dt.formateToString() + UserfulFunctions::getSuffix(strTraceB);

    //消息相关设置
    QString info;
    /*Marker 的设置*/
    QString label;

    bool blYLeftON=this->axisEnabled(QwtPlot::yLeft);
    bool blYRightON=this->axisEnabled(QwtPlot::yRight);
    if(blYLeftON && blYRightON)
    {

        info= QString("%1=%2, %3=%4, %5=%6")
                .arg(strXTrace).arg(strX)
                .arg(strTraceA).arg(strItem1)
                .arg(strTraceB).arg(strItem2);

        label= QString("<p> <font size=\"3\" face=\"Helvetica\" color=\"#00c000\">%1<br>").arg(strX)+
                QString(" <font size=\"3\" face=\"Helvetica\" color=\"cyan\">%1").arg(strItem1)+
                QString(" <font size=\"3\" face=\"Helvetica\" color=\"Gold\">%1</p>").arg(strItem2);
    }
    else if(blYLeftON && (!blYRightON))
    {
        info= QString("%1=%2, %3=%4")
                .arg(strXTrace).arg(strX)
                .arg(strTraceA).arg(strItem1);

        label= QString("<p> <font size=\"3\" face=\"Helvetica\" color=\"#00c000\">%1<br>").arg(strX)+
                QString(" <font size=\"3\" face=\"Helvetica\" color=\"cyan\">%1</p>").arg(strItem1);
    }
    else if((!blYLeftON) && (blYRightON))
    {
        info= QString("%1=%2, %3=%4")
                .arg(strXTrace).arg(strX)
                .arg(strTraceB).arg(strItem2);

        label= QString("<p> <font size=\"3\" face=\"Helvetica\" color=\"#00c000\">%1<br>").arg(strX)+
                QString(" <font size=\"3\" face=\"Helvetica\" color=\"Gold\">%1</p>").arg(strItem2);

    }
    else
    {
        info= QString("%1=%2")
                .arg(strXTrace).arg(strX);
        label= QString("<p> <font size=\"3\" face=\"Helvetica\" color=\"#00c000\">%1</p>").arg(strX);

    }
    QwtText text(label);
    text.setFont(QFont("Helvetica", 10, QFont::Bold));
    text.setColor(Qt::green);

    int index=keys.indexOf(freqValue);

    if(index <keys.length()*3/6)
        d_marker1->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    else/*(index >keys.length()*3/6)*/
        d_marker1->setLabelAlignment(Qt::AlignLeft |Qt::AlignBottom);

    d_marker1->setLabel(text);
    d_marker1->setTitle("1");

    return info;
}

bool Plot::getBlLogYLeft() const
{
    return blLogYLeft;
}

void Plot::setBlLogYLeft(bool value)
{
    blLogYLeft = value;
    if(blLogYLeft)
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLog10ScaleEngine );
    else
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLinearScaleEngine);
}

bool Plot::getBlLogX() const
{
    return blLogX;
}

void Plot::setBlLogX(bool value)
{
    blLogX = value;
    if(blLogX)
        this->setAxisScaleEngine(QwtPlot::xBottom,
                                 new QwtLog10ScaleEngine);
    else
        this->setAxisScaleEngine(QwtPlot::xBottom,
                                 new QwtLinearScaleEngine);

}

void Plot::setToTop(curveProperty property, bool isOnTop)
{
   if(curves.keys().contains(property.index))
   {
       curves.value(property.index).cur1->setZ((isOnTop?30:20));
       curves.value(property.index).cur2->setZ((isOnTop?30:20));
   }
}

double Plot::getYLeftMax() const
{
    return this->axisScaleDiv(QwtPlot::yLeft)->upperBound();
}

double Plot::getYLeftMin() const
{
    return this->axisScaleDiv(QwtPlot::yLeft)->lowerBound();
}

double Plot::getXmin() const
{
    return this->axisScaleDiv(QwtPlot::xBottom)->lowerBound();
}


double Plot::getXmax() const
{
    return this->axisScaleDiv(QwtPlot::xBottom)->upperBound();
}

double Plot::getYRightMax() const
{
    return this->axisScaleDiv(QwtPlot::yRight)->upperBound();
}

double Plot::getYRightMin() const
{
    return this->axisScaleDiv(QwtPlot::yRight)->lowerBound();
}


void Plot::turnOffRefTrace()
{
    QList<int> keys = curves.keys();

    for(int i=0; i< keys.length(); i++)
    {
        qDebug()<<"curves "<<keys.at(i);
        if(keys.at(i)!=0)
        {
            curves.value(keys.at(i)).cur1->detach();
            curves.value(keys.at(i)).cur2->detach();
            curves.remove(keys.at(i));
        }
    }
    this->replot();
}
