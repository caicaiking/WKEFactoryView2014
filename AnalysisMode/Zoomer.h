#ifndef ZOOMER_H
#define ZOOMER_H
#include "Qwt/qwt_plot_zoomer.h"
class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
        QwtPlotZoomer(xAxis, yAxis, canvas)
    {
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBand(QwtPicker::NoRubberBand);

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

//        setMousePattern(QwtEventPattern::MouseSelect2,
//            Qt::RightButton, Qt::ControlModifier);
//        setMousePattern(QwtEventPattern::MouseSelect3,
//            Qt::RightButton);
    }
};


#endif // ZOOMER_H
