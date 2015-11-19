#ifndef CLSMYBARCHARTPLOT_H
#define CLSMYBARCHARTPLOT_H

#include <QWidget>
#include <QPixmap>
#include <QMap>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include "PlotSettings.h"
#include "PlotDataAnlysis.h"

class clsMyBarChartPlot : public QWidget
{
    Q_OBJECT
public:
    explicit clsMyBarChartPlot(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void refreshPixmap();
    void setTitle(const QString &t1, const QString &t2, const QString &t3);
    void setPlotSettings(const PlotSettings &settings);
    void setPlotDataAnalysis(const PlotDataAnlysis &data);
    void updateChart();


    double getMeanValue() const;
    double getStdValue() const;
    double getNeg3Std() const;
    double getPos3Std() const;


protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void repfreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurve(QPainter *painter);
    void drawBar(QPainter *painter);
private:
    enum {
        Margin = 50,
          XMargin = (int)(1.5*Margin),
          yMargin = (int)(0.5*Margin),
          widthMargin = (int)(2*Margin),
          heightMargin = (int)(1.5*Margin)
    };

    QMap <int ,QVector<QPointF> > curveMap;
    QPixmap pixmap;
    PlotSettings pltSettings;
    QString xTitle, yTitle, zTitle;
    PlotDataAnlysis testData;
};

#endif // CLSMYBARCHARTPLOT_H
