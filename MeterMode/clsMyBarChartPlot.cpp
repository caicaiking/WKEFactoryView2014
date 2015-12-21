#include "clsMyBarChartPlot.h"

#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <QDebug>
#include "doubleType.h"
#include <QRect>
clsMyBarChartPlot::clsMyBarChartPlot(QWidget *parent) : QWidget(parent)
{
    this->setBackgroundRole(QPalette::Dark);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // this->setFocusPolicy(Qt::StrongFocus);
    this->xTitle="Untitled";
    this->yTitle="Untitled";
    this->zTitle="Untitled";
    this->resize(this->minimumSizeHint()); //给一个基础的宽度和高度
}

QSize clsMyBarChartPlot::sizeHint() const
{
    return QSize(8 *Margin, 12 * Margin);
}

QSize clsMyBarChartPlot::minimumSizeHint() const
{
    return QSize(550,450);
}

void clsMyBarChartPlot::paintEvent(QPaintEvent */*event*/)
{

    QStylePainter painter(this);
    painter.drawPixmap(0,0,pixmap);

    refreshPixmap();

}

void clsMyBarChartPlot::refreshPixmap()
{
    pixmap = QPixmap(this->size());
    pixmap.fill(QColor("black"));

    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawBar(&painter);
    drawCurve(&painter);
    // this->update();
}

void clsMyBarChartPlot::setTitle(const QString &t1, const QString &t2, const QString &t3)
{
    this->xTitle=t1;
    this->yTitle=t2;
    this->zTitle=t3;
    this->update();
}

void clsMyBarChartPlot::setPlotSettings(const PlotSettings &settings)
{
    this->pltSettings = settings;
    refreshPixmap();
}

void clsMyBarChartPlot::setPlotDataAnalysis(const PlotDataAnlysis &data)
{
    this->testData = data;
    this->setPlotSettings(testData.getSettings());
    this->update();
}


void clsMyBarChartPlot::resizeEvent(QResizeEvent */*event*/)
{
    this->update();
}

void clsMyBarChartPlot::mouseMoveEvent(QMouseEvent *event)
{

}

void clsMyBarChartPlot::drawGrid(QPainter *painter)
{
    QRect rect(XMargin,yMargin,this->width()-widthMargin,this->height()-heightMargin);
    if(!rect.isValid())
        return;
    //绘制X轴的坐标系，并添加数字标签
    if(pltSettings.numXTicks.length()>=2)
    {
        double littleTick = (pltSettings.numXTicks.at(1)-pltSettings.numXTicks.at(0))/2.0;
        int i=0;
        while((pltSettings.numXTicks.at(0)- 1.0*i* littleTick)>=pltSettings.minX)
        {
            int x = rect.left()+((pltSettings.numXTicks.at(0)- 1.0*i* littleTick
                                  -pltSettings.minX)/pltSettings.spanX())*rect.width();

            QPen pen(QColor::fromRgb(0,96,0), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
            painter->setPen(pen);
            painter->drawLine(x, rect.top(), x, rect.bottom());

            painter->drawLine(x, rect.bottom(), x, rect.bottom() + 4);
            i++;
        }
        i=0;
        while((pltSettings.numXTicks.at(0)+1.0*i* littleTick)<=pltSettings.maxX)
        {
            int x = rect.left()+((pltSettings.numXTicks.at(0)+ 1.0*i* littleTick
                                  -pltSettings.minX)/pltSettings.spanX())*rect.width();

            QPen pen(QColor::fromRgb(0,96,0), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
            painter->setPen(pen);
            painter->drawLine(x, rect.top(), x, rect.bottom());
            painter->drawLine(x, rect.bottom(), x, rect.bottom() + 4);
            i++;
        }

    }

    //qDebug()<< pltSettings.numXTicks.length();
    for(int i=0; i< pltSettings.numXTicks.length(); i++)
    {
        int x = rect.left() + ((pltSettings.numXTicks.at(i)-pltSettings.minX)/ pltSettings.spanX())
                * rect.width();
        double label = pltSettings.numXTicks.at(i);
        QPen pen(QColor::fromRgb(0,192,0), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(QColor::fromRgb(0,192,0));
        painter->setFont(QFont("楷体",9));
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 6);
        doubleType dt;
        dt.setData(label,"");
        painter->drawText(x - 50, rect.bottom() + 6, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          dt.formateToString(6));
    }

    //绘制X轴标题
    painter->save();
    painter->setFont(QFont("楷体",13));
    painter->drawText(XMargin,yMargin+rect.height()+20,rect.width(),20,Qt::AlignHCenter | Qt::AlignTop,xTitle);

    painter->restore();


    //绘制y轴的坐标系，并添加数字标签
    if(pltSettings.numYTicks.length()>=2)
    {
        double littleTick = (pltSettings.numYTicks.at(1)-pltSettings.numYTicks.at(0))/2.0;
        int i=0;
        while((pltSettings.numYTicks.at(0)- 1.0*i* littleTick)>=pltSettings.minY)
        {
            int y = rect.bottom()-((pltSettings.numYTicks.at(0)- 1.0*i* littleTick
                                    -pltSettings.minY)/pltSettings.spanY())*rect.height();

            QPen pen(QColor::fromRgb(0,64,64), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
            painter->setPen(pen);
            painter->drawLine(rect.left(),y, rect.right(), y);

            painter->drawLine(rect.left()-4, y, rect.left(), y);
            i++;
        }
        i=0;
        while((pltSettings.numYTicks.at(0)+1.0*i* littleTick)<=pltSettings.maxY)
        {
            int y = rect.bottom()-((pltSettings.numYTicks.at(0)+ 1.0*i* littleTick
                                    -pltSettings.minY)/pltSettings.spanY())*rect.height();

            QPen pen(QColor::fromRgb(0,64,64), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
            painter->setPen(pen);
            painter->drawLine(rect.left(),y, rect.right(), y);

            painter->drawLine(rect.left()-4, y, rect.left(), y);
            i++;
        }

    }

    for(int i=0; i< pltSettings.numYTicks.length(); i++)
    {
        int y = rect.bottom()-((pltSettings.numYTicks.at(i)-pltSettings.minY)/pltSettings.spanY())*rect.height();
        double label = pltSettings.numYTicks.at(i);
        QPen pen(QColor::fromRgb(0,96,90), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawLine(rect.left(),y, rect.right(), y);

        painter->setPen(QColor::fromRgb(0,96,96));
        painter->setFont(QFont("楷体",9));
        painter->drawLine(rect.left()-6, y, rect.left(), y);
        doubleType dt;
        dt.setData(label,"");
        painter->drawText(rect.left() - widthMargin, y - 10, widthMargin - 6, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          dt.formateToString(6));


    }
    //绘制Y轴标题
    painter->save();
    painter->rotate(-90.0);
    painter->setFont(QFont("楷体",13));

    QPointF p1(-yMargin,6);
    QPointF p2(-yMargin-rect.height(),20.0);
    QRectF rect1(p1,p2);

    painter->drawText(rect1,Qt::AlignHCenter| Qt::AlignVCenter,yTitle);

    painter->restore();

    QPen pen(QColor::fromRgb(96,128,80));
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawRect(rect.adjusted(-1,-1,2,-1));

    painter->setFont(QFont("楷体",13));
    painter->drawText(XMargin,4,rect.width(),20,Qt::AlignHCenter | Qt::AlignTop,zTitle);

}

void clsMyBarChartPlot::drawCurve(QPainter *painter)
{
    QRect rect(XMargin,yMargin,this->width()-widthMargin,this->height()-heightMargin);
    rect =rect.adjusted(+1, +1, -1, -1);

    if(!rect.isValid())
        return;

    painter->save();
    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    painter->setPen(pen);
    QVector<QPointF> data = testData.getFitData();
    QPointF last;
    for(int i=0; i< data.length(); i++)
    {
        int xValue = rect.left() + ((data.at(i).x()-pltSettings.minX)/ pltSettings.spanX())*rect.width();
        int yValue = rect.bottom() -
                ((data.at(i).y()-pltSettings.minY)/pltSettings.spanY())*rect.height();

        if(i==0)
            painter->drawPoint(xValue,yValue);
        else
            painter->drawLine(last,QPointF(xValue,yValue));

        last.setX(xValue);
        last.setY(yValue);

    }

    painter->restore();
}

void clsMyBarChartPlot::drawBar(QPainter *painter)
{

    QVector<SINGLE_BAR> data = testData.getBars();
    QRect rect(XMargin,yMargin,this->width()-widthMargin,this->height()-heightMargin);
    rect =rect.adjusted(+1, +1, -1, -1);

    if(!rect.isValid())
        return;


    for(int i=0; i<data.length();i++)
    {
       // qDebug()<< data.at(i).barValue;
        int start = rect.left() + ((data.at(i).x-pltSettings.minX)/ pltSettings.spanX())*rect.width();
        int stop = rect.left() + ((data.at(i).y-pltSettings.minX)/ pltSettings.spanX())*rect.width();
        int barValue = rect.bottom() -
                ((data.at(i).barValue-pltSettings.minY)/pltSettings.spanY())*rect.height();


        painter->fillRect(start,barValue,stop-start,rect.bottom()-barValue,QColor::fromRgb(192,80,77));
        painter->drawRect(start,barValue,stop-start,rect.bottom()-barValue);
    }


}

void clsMyBarChartPlot::updateChart()
{
    this->repaint();
}

double clsMyBarChartPlot::getMeanValue() const
{
    return testData.getMeanValue();
}

double clsMyBarChartPlot::getStdValue() const
{
    return testData.getStdValue();
}

double clsMyBarChartPlot::getNeg3Std() const
{
    return testData.getNeg3Std();
}

double clsMyBarChartPlot::getPos3Std() const
{
    return testData.getPos3Std();
}



