#ifndef MULTIMODEPLOT_H
#define MULTIMODEPLOT_H

#include <QObject>
#include <QWidget>
#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_curve.h>
#include <QVector>
#include <Qwt/qwt_symbol.h>
class TestCurve
{
public:
    TestCurve()
    {
        index=0;
    }

    /*!
     * \brief 清理数据
     */
    void clearData()
    {
        data.clear();
    }

    void clearData(int /*i*/)
    {
        QList<int> keys = data.keys();
        for(int i=0; i< keys.length(); i++)
        {
            data[keys.at(i)].clear();
        }
    }

    /*！
     *清理点数
     */
    void clearPoint()
    {
        points.clear();
    }

    /*!
     * \brief 设置测试点
     * \param 点的值
     */
    void setPoint(double value)
    {
        points.append(value);
    }
    /*!
     * \brief 设置测试曲线
     * \param 曲线的值
     */
    void setCurve(QwtPlotCurve * value)
    {
        this->curve = value;
    }
    /*!
     * \brief 获取测试曲线
     * \return 返回测试曲线的引用
     */
    QwtPlotCurve *getCurve()
    {
        return this->curve;
    }

    /*!
     * \brief 设置曲线的可见性
     * \param true 可见 false 不可见
     */
    void setVisable(bool value)
    {
        curve->setVisible(value);
    }

    /*!
     * \brief 设置曲线显示的数据
     * \param value 数据索引
     */
    void setDataIndex(int value)
    {
        this->index = value;
        if(index < data.count())
            curve->setSamples(points, data[index]);
    }

    /*!
     * \brief 将点幅值个曲线
     * \param 点
     * \param 测试的数据
     */
    void setData(double point, QList<double>data1)
    {
        if(data.count() != data1.length() )
            return;


        points.append(point);



        for(int i=0; i<data1.length(); i++)
        {
            data[i].append(data1.at(i));
            //qDebug()<<i;
        }
        curve->setSamples(points, data[index]);
    }
    /*!
     * \brief 初始化数据，先将数据扩成容量
     * \param 第几组
     * \param 数据
     */
    void initData(int key, QVector<double> data1)
    {
        data.insert(key, data1);
    }

    /*!
     * \brief 设置曲线的颜色
     * \param 颜色名称
     */
    void setColor(QString colorName)
    {
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setPen(QPen(QColor(colorName)));
        curve->setStyle(QwtPlotCurve::Lines);
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
    void clearData();//清除所有的测试数据
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
