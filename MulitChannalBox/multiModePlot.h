#ifndef MULTIMODEPLOT_H
#define MULTIMODEPLOT_H

#include <QObject>
#include <QWidget>
#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_curve.h>
#include <QVector>
class TestCurve
{
public:
    TestCurve()
    {
        index=0;
    }

    void clearData()
    {
        data.clear();
    }

    void clearPoint()
    {
        points.clear();
    }

    void setPoint(double value)
    {
        points.append(value);
    }

    void setCurve(QwtPlotCurve * value)
    {
        this->curve = value;
    }

    QwtPlotCurve *getCurve()
    {
        return this->curve;
    }

    void setVisable(bool value)
    {
        curve->setVisible(value);
    }

    void setDataIndex(int value)
    {
        this->index = value;
    }

    void setData(double point, QList<double>data1)
    {
        if(data.count() != data1.length() )
            return;

        points.append(point);

        for(int i=0; i<data1.length(); i++)
        {
            data[i].append(data1.at(i));
            qDebug()<<i;
        }
        curve->setSamples(points, data[index]);
    }

    void initData(int key, QVector<double> data1)
    {
        data.insert(key, data1);
    }

    void setColor(QString colorName)
    {
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setPen(QPen(QColor(colorName)));
        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curve->setYAxis(QwtPlot::yLeft);
    }

private:
    int index;
    QwtPlotCurve *curve;
    QVector<double> points;
    QMap<int, QVector<double> > data;

};

class multiModePlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit multiModePlot(QWidget *parent = 0);

    void setChannalAndItems(QString channel, QStringList items);
    void setCurveVisable(QList<bool> value);
    void setCurrentIndex(int index);
    void setXTrace(const QString &value);
    void setData(int channal, double point, QList<double> data);
    void setGraphTitle(const QString &strTitle);
signals:

public slots:

private:
    QMap<int, TestCurve> testData;
};

#endif // MULTIMODEPLOT_H
