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
#include <Qwt/qwt_plot_canvas.h>
#include <QMapIterator>
#include <Qwt/qwt_plot_canvas.h>
Plot::Plot(QWidget *parent) :
    QwtPlot(parent)
{
    this->setAutoReplot(false);
    setGraphTitle("Unititled");


    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius(5 );

    setCanvas( canvas );
    setCanvasBackground(QColor(Qt::black));


    //this->setStyleSheet("background-color: black;");
    blLogX=true;
    blLogYLeft=true;


    //y axis Left grid
    gridYLeft=new QwtPlotGrid;
    gridYLeft->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    gridYLeft->setMajorPen(QPen(QColor(0,128,128),0,Qt::DotLine));
    gridYLeft->setMinorPen(QPen(QColor(0,64,64),0,Qt::DotLine));
    gridYLeft->enableYMin(true);
    gridYLeft->enableX(false);
    gridYLeft->attach(this);

    //y axis Right grid
    gridYRight=new QwtPlotGrid;
    gridYRight->setAxes(QwtPlot::xBottom,QwtPlot::yRight);
    gridYRight->setMajorPen(QPen(QColor(96,96,0),0,Qt::DotLine));
    gridYRight->setMinorPen(QPen(QColor(96,96,0),0,Qt::DotLine));
    gridYRight->enableYMin(false);
    gridYRight->enableX(false);
    gridYRight->attach(this);

    //x axis grid
    gridX=new QwtPlotGrid;
    gridX->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    gridX->enableY(false);
    gridX->enableYMin(false);
    gridX->enableXMin(true);
    gridX->setMajorPen(QPen(QColor(0,192,0),0,Qt::DotLine));
    gridX->setMinorPen(QPen(QColor(0,96,0),0,Qt::DotLine));
    gridX->attach(this);



    this->enableAxis(QwtPlot::yLeft,true);
    this->enableAxis(QwtPlot::yRight);
    setTraces("Z",QString("A"),tr("频率"));
    this->setAxisMaxMajor(QwtPlot::xBottom,6);
    this->setAxisMaxMajor(QwtPlot::yLeft,10);
    if(blLogX)
        this->setAxisScaleEngine(QwtPlot::xBottom,
                                 new QwtLogScaleEngine);
    else
        this->setAxisScaleEngine(QwtPlot::xBottom,
                                 new QwtLinearScaleEngine);
    if(blLogYLeft)
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLogScaleEngine );
    else
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLinearScaleEngine);

    this->setAxisScale(QwtPlot::xBottom,10,1000);
    this->setAxisScale(QwtPlot::yLeft,1,1000);
    this->setAxisScale(QwtPlot::yRight,1,10);

    this->setAxisFont(QwtPlot::yLeft,QFont("Times", 10));
    this->setAxisFont(QwtPlot::yRight,QFont("Times", 10));
    this->setAxisFont(QwtPlot::xBottom,QFont("Times", 10));

    //init 10 Markers
    for(int i =0; i< 10; i++)
    {
        QwtPlotMarker * tmpMarker = new QwtPlotMarker();
        tmpMarker->setLineStyle(QwtPlotMarker::VLine);
        tmpMarker->setLabelAlignment(Qt::AlignRight |
                                     Qt::AlignBottom);
        tmpMarker->setLinePen(QPen(Qt::red,0,Qt::SolidLine));

        tmpMarker->attach(this);
        tmpMarker->setVisible(false);
        tmpMarker->setTitle(QString::number(i));
        markers.append(tmpMarker);
    }

    setCurrentMarker(0);




    //上下限设定的Mark线
    traceAUpLimit = new QwtPlotMarker;
    traceADownLimit = new QwtPlotMarker;
    traceBUpLimit = new QwtPlotMarker;
    traceBDownLimit = new QwtPlotMarker;


    this->setAutoReplot(true);
    setTraceA(QString("Z"));
    setTraceB(QString("A"));
    setXTrace(QString(tr("频率")));
}


void Plot::mouseMoveEvent(QMouseEvent */*e*/)
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
    d_marker1->setVisible(false);

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
    this->d_marker1->setVisible(false);
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
        curves.value(property.index).cur1->setSamples(UserfulFunctions::getPlotCurveData(curves.value(0).cur1).toVector());
        curves.value(property.index).cur2->setSamples(UserfulFunctions::getPlotCurveData(curves.value(0).cur2).toVector());
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
    setCurveLimit(this->curveLimit);
}

void Plot::setYLeftScal(double min, double max, bool logx)
{
    this->setAxisScale(QwtPlot::yLeft,min,max);
    this->yLeftMin =min;
    this->yLeftMax=max;
    setBlLogYLeft(logx);
    setCurveLimit(this->curveLimit);
}

void Plot::setYRightScal(double min, double max)
{
    this->yRightMax=max;
    this->yRightMin =min;
    this->setAxisScale(QwtPlot::yRight,min,max);
    setCurveLimit(this->curveLimit);
}

void Plot::setScale(double xmin, double xmax, bool blLogX,
                    double yLeftMin, double yLeftMax, bool blLogYLeft,
                    double yRightMin, double yRightMax)
{
    setXScale(xmin,xmax,blLogX);
    setYLeftScal(yLeftMin,yLeftMax,blLogYLeft);
    setYRightScal(yRightMin,yRightMax);
    setCurveLimit(this->curveLimit);
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
        {
            gridYLeft->attach(this);
        }
        else
        {
            gridYLeft->detach();
        }

        break;
    case yRight:
        this->enableAxis(QwtPlot::yRight,status);
        if(status)
        {
            gridYRight->attach(this);
        }
        else
        {
            gridYRight->detach();
        }
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
    myPrivateData = UserfulFunctions::getPlotCurveData(curves.values(),0);

    QList<double> keys= myPrivateData.keys();
    //找到数组中的频点，然后在获取相关的数值
    double freqValue, item1, item2;
    if(freq>UserfulFunctions::Max(keys))
        freqValue= UserfulFunctions::Max(keys);
    else if(freq<UserfulFunctions::Min(keys))
        freqValue= UserfulFunctions::Min(keys);
    else
        freqValue= UserfulFunctions::getNearItem(keys,freq,blLogYLeft);

    dblMarkValue=freqValue;
    item1=((QPointF) myPrivateData.value(freqValue)).x();
    item2=((QPointF) myPrivateData.value(freqValue)).y();
    d_marker1->setVisible(true);
    d_marker1->setValue(freqValue,0.0);

    QString msg= QString("%1,%2,%3").arg(QString::number(freqValue))
            .arg(QString::number(item1)).arg(QString::number(item2));

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
                QString(" <font size=\"3\" face=\"Helvetica\" color=\"cyan\">%1<br>").arg(strItem1)+
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

    if(!multiMarker)
    {
        if(index <keys.length()*3/6)
            d_marker1->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
        else/*(index >keys.length()*3/6)*/
            d_marker1->setLabelAlignment(Qt::AlignLeft |Qt::AlignBottom);
        d_marker1->setLabel(text);
        d_marker1->setTitle("1");
    }
    else
    {
        if(index <keys.length()*3/6)
            d_marker1->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
        else/*(index >keys.length()*3/6)*/
            d_marker1->setLabelAlignment(Qt::AlignLeft |Qt::AlignTop);
        d_marker1->setLabel(QString::number(this->currentMarker+1));
        d_marker1->setTitle(QString::number(this->currentMarker+1));
        emit showMarkerMessage(this->currentMarker, text.text());
    }

    // return info;
    return msg;
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
                                 new QwtLogScaleEngine );
    else
        this->setAxisScaleEngine(QwtPlot::yLeft,
                                 new QwtLinearScaleEngine);
}

bool Plot::getYLeftIsEnable()
{
    return this->axisEnabled(QwtPlot::yLeft);
}

bool Plot::getYRightIsEnable()
{
    return this->axisEnabled(QwtPlot::yRight);
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
                                 new QwtLogScaleEngine);
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
    return this->axisScaleDiv(QwtPlot::yLeft).upperBound();
}

double Plot::getYLeftMin() const
{
    return this->axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

double Plot::getXmin() const
{
    return this->axisScaleDiv(QwtPlot::xBottom).lowerBound();
}


double Plot::getXmax() const
{
    return this->axisScaleDiv(QwtPlot::xBottom).upperBound();
}

double Plot::getYRightMax() const
{
    return this->axisScaleDiv(QwtPlot::yRight).upperBound();
}

double Plot::getYRightMin() const
{
    return this->axisScaleDiv(QwtPlot::yRight).lowerBound();
}


void Plot::turnOffRefTrace()
{
    QList<int> keys = curves.keys();

    for(int i=0; i< keys.length(); i++)
    {
        //qDebug()<<"curves "<<keys.at(i);
        if(keys.at(i)!=0)
        {
            curves.value(keys.at(i)).cur1->detach();
            curves.value(keys.at(i)).cur2->detach();
            curves.remove(keys.at(i));
        }
    }
    this->replot();
}


void Plot::findPeak(Choice x,bool peakType)
{
    QMap<double,QPointF> myPrivateData;
    myPrivateData = UserfulFunctions::getPlotCurveData(curves.values(),0);

    QMap<double,double> item;

    QMapIterator <double, QPointF> it(myPrivateData);

    while(it.hasNext())
    {
        it.next();
        switch (x)
        {
        case yLeft:
            item.insert(it.key(),it.value().x());
            break;
        case yRight:
            item.insert(it.key(),it.value().y());
            break;
        case Both:
            break;
        }
    }

    double value=0;

    if(peakType)
    {
        value = UserfulFunctions::Max(item.values());
    }
    else
    {
        value = UserfulFunctions::Min(item.values());
    }

    //qDebug()<< "Freq: " << item.key(value);

    setMarker(item.key(value),0);

}

double Plot::getCurrentMarkValue()
{
    double tmpRet =  d_marker1->value().x();

    return tmpRet;
}

void Plot::setCurveLimit(const clsCurveLimit &curveLimit1)
{
    this->curveLimit = curveLimit1;



    if(this->curveLimit.blTraceALimit)
    {

        traceAUpLimit->setValue(0.0,curveLimit.cmlTraceALimit.getAbsLimitHigh());
        traceAUpLimit->setLineStyle(QwtPlotMarker::HLine);
        traceAUpLimit->setLabelAlignment(Qt::AlignLeft |
                                         Qt::AlignTop);
        traceAUpLimit->setLinePen(QPen(curveLimit.traceAUp,0,Qt::DashDotLine));
        // traceAUpLimit->setLabel(tr("上限"));
        traceAUpLimit->attach(this);
        traceAUpLimit->setVisible(true);

        traceADownLimit->setValue(0.0,curveLimit.cmlTraceALimit.getAbsLimitLow());
        traceADownLimit->setLineStyle(QwtPlotMarker::HLine);
        traceADownLimit->setLabelAlignment(Qt::AlignLeft |
                                           Qt::AlignBottom);
        traceADownLimit->setLinePen(QPen(curveLimit.traceADown,0,Qt::DashDotLine));
        // traceADownLimit->setLabel(tr("下限"));
        traceADownLimit->attach(this);
        traceADownLimit->setVisible(true);

    }
    else
    {
        traceAUpLimit->setVisible(false);
        traceADownLimit->setVisible(false);
    }

    if(this->curveLimit.blTraceBLimit)
    {
        double yRightHi;
        yRightHi=this->canvasMap(QwtPlot::yRight).transform(this->curveLimit.cmlTraceBLimit.getAbsLimitHigh());
        yRightHi=this->canvasMap(QwtPlot::yLeft).invTransform(yRightHi);
        traceBUpLimit->setValue(0.0,yRightHi);
        traceBUpLimit->setLineStyle(QwtPlotMarker::HLine);
        traceBUpLimit->setLabelAlignment(Qt::AlignLeft |
                                         Qt::AlignTop);
        traceBUpLimit->setLinePen(QPen(curveLimit.traceBUp,0,Qt::DashLine));
        // traceAUpLimit->setLabel(tr("上限"));
        traceBUpLimit->attach(this);
        traceBUpLimit->setVisible(true);

        double yRightLow;
        yRightLow=this->canvasMap(QwtPlot::yRight).transform(this->curveLimit.cmlTraceBLimit.getAbsLimitLow());
        yRightLow=this->canvasMap(QwtPlot::yLeft).invTransform(yRightLow);


        traceBDownLimit->setValue(0.0,yRightLow);
        traceBDownLimit->setLineStyle(QwtPlotMarker::HLine);
        traceBDownLimit->setLabelAlignment(Qt::AlignLeft |
                                           Qt::AlignBottom);
        traceBDownLimit->setLinePen(QPen(curveLimit.traceBDown,0,Qt::DashLine));
        // traceADownLimit->setLabel(tr("下限"));
        traceBDownLimit->attach(this);
        traceBDownLimit->setVisible(true);

    }
    else
    {
        traceBUpLimit->setVisible(false);
        traceBDownLimit->setVisible(false);
    }

    this->replot();
}

void Plot::setCurveLimitVisiable(const bool &value)
{
    traceAUpLimit->setVisible(value);
    traceADownLimit->setVisible(value);
    traceBDownLimit->setVisible(value);
    traceBUpLimit->setVisible(value);
}

void Plot::setMultMarker(bool value)
{
    this->multiMarker= value;
}

void Plot::setCurrentMarker(int value)
{
    this->currentMarker = value;
    d_marker1 = markers.at(value);
}

void Plot::setMarkerVisual(int i, bool value)
{
   markers.at(i)->setVisible(value);
}


void Plot::findNextLeftHigh(Choice x)
{
    if(!d_marker1->isVisible())
        return;

    double tmpRet = getCurrentMarkValue();

    QMap<double,QPointF> myPrivateData;
    myPrivateData = UserfulFunctions::getPlotCurveData(curves.values(),0);

    QMap<double,double> list;

    QMapIterator <double, QPointF> it1(myPrivateData);

    while(it1.hasNext())
    {
        it1.next();
        switch (x)
        {
        case yLeft:
            list.insert(it1.key(),it1.value().x());
            break;
        case yRight:
            list.insert(it1.key(),it1.value().y());
            break;
        case Both:
            break;
        }
    }

    QMap <double,double> tmpMap;

    QMapIterator<double, double> it(list);

    while(it.hasNext())
    {
        it.next();

        if(it.key()!= tmpRet)
        {
            tmpMap.insert(it.key(),it.value());
        }
        else
        {
            tmpMap.insert(it.key(),it.value());
            break;
        }
    }

    QList<double> values = tmpMap.values();
    QList <double> keys = tmpMap.keys();
    for(int i= tmpMap.count()-2; i>=1; i--)
    {
        if((values.at(i+1)<values.at(i)) && (values.at(i-1)<values.at(i)))
        {
            setMarker(keys.at(i),0);
            return;
        }
    }

    this->setMarker(tmpRet,0);


}

void Plot::findNextLeftLow(Choice x)
{
    if(!d_marker1->isVisible())
        return;

    double tmpRet = getCurrentMarkValue();

    QMap<double,QPointF> myPrivateData;
    myPrivateData = UserfulFunctions::getPlotCurveData(curves.values(),0);

    QMap<double,double> list;

    QMapIterator <double, QPointF> it1(myPrivateData);

    while(it1.hasNext())
    {
        it1.next();
        switch (x)
        {
        case yLeft:
            list.insert(it1.key(),it1.value().x());
            break;
        case yRight:
            list.insert(it1.key(),it1.value().y());
            break;
        case Both:
            break;
        }
    }



    QMap <double,double> tmpMap;

    QMapIterator<double, double> it(list);

    while(it.hasNext())
    {
        it.next();

        if(it.key()!= tmpRet)
        {
            tmpMap.insert(it.key(),it.value());
        }
        else
        {
            tmpMap.insert(it.key(),it.value());
            break;
        }

    }

    QList<double> values = tmpMap.values();
    QList <double> keys = tmpMap.keys();
    for(int i= tmpMap.count()-2; i>=1; i--)
    {
        if((values.at(i+1)>values.at(i)) && (values.at(i-1)>values.at(i)))
        {
            setMarker(keys.at(i),0);
            return;
        }
    }

    this->setMarker(tmpRet,0);

}

void Plot::findNextRightHigh(Choice x)
{
    if(!d_marker1->isVisible())
        return;
    double tmpRet = getCurrentMarkValue();

    QMap<double,QPointF> myPrivateData;
    myPrivateData = UserfulFunctions::getPlotCurveData(curves.values(),0);

    QMap<double,double> list;

    QMapIterator <double, QPointF> it1(myPrivateData);

    while(it1.hasNext())
    {
        it1.next();
        switch (x)
        {
        case yLeft:
            list.insert(it1.key(),it1.value().x());
            break;
        case yRight:
            list.insert(it1.key(),it1.value().y());
            break;
        case Both:
            break;
        }
    }



    QMap <double,double> tmpMap;

    QMapIterator<double, double> it(list);

    while(it.hasNext())
    {
        it.next();

        if(it.key()>= tmpRet)
        {
            tmpMap.insert(it.key(),it.value());
        }
    }

    QList<double> values = tmpMap.values();
    QList <double> keys = tmpMap.keys();

    for(int i= 1; i<=tmpMap.count() -2; i++)
    {
        if((values.at(i+1)<values.at(i)) && (values.at(i-1)<values.at(i)))
        {
            setMarker(keys.at(i),0);
            return;
        }
    }

    setMarker(tmpRet,0);

}

void Plot::findNextRightLow(Choice x)
{
    if(!d_marker1->isVisible())
        return;

    double tmpRet = getCurrentMarkValue();

    QMap<double,QPointF> myPrivateData;
    myPrivateData = UserfulFunctions::getPlotCurveData(curves.values(),0);

    QMap<double,double> list;

    QMapIterator <double, QPointF> it1(myPrivateData);

    while(it1.hasNext())
    {
        it1.next();
        switch (x)
        {
        case yLeft:
            list.insert(it1.key(),it1.value().x());
            break;
        case yRight:
            list.insert(it1.key(),it1.value().y());
            break;
        case Both:
            break;
        }
    }

    QMap <double,double> tmpMap;
    QMapIterator<double, double> it(list);
    while(it.hasNext())
    {
        it.next();

        if(it.key()>= tmpRet)
        {
            tmpMap.insert(it.key(),it.value());
        }
    }

    QList<double> values = tmpMap.values();
    QList <double> keys = tmpMap.keys();

    for(int i= 1; i<=tmpMap.count() -2; i++)
    {
        if((values.at(i+1)>values.at(i)) && (values.at(i-1)>values.at(i)))
        {
            setMarker(keys.at(i),0);
            return;
        }
    }

    setMarker(tmpRet,0);
}

