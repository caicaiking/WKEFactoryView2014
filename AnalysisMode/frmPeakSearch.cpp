#include "frmPeakSearch.h"

frmPeakSearch::frmPeakSearch(Plot *plot, const graphSetup gs, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setPlot(plot);
    setGraphSetup(gs);

    btnItem1->setEnabled(plot->getYLeftIsEnable());
    btnItem2->setEnabled(plot->getYRightIsEnable());

    btnItem1->setText(plot->getTraceA());
    btnItem2->setText(plot->getTraceB());

}

void frmPeakSearch::setPlot(Plot *plot)
{
    this->plot = plot;
}

void frmPeakSearch::setGraphSetup(const graphSetup gs)
{
    this->gs=gs;
}


void frmPeakSearch::on_btnItem1_clicked()
{
    choice =Plot::yLeft;
}

void frmPeakSearch::on_btnItem2_clicked()
{
    choice = Plot::yRight;
}

void frmPeakSearch::on_btnMax_clicked()
{
    plot->findPeak(choice,true);
}

void frmPeakSearch::on_btnMin_clicked()
{
    plot->findPeak(choice,false);
}

void frmPeakSearch::on_btnExit_clicked()
{
    this->accept();
}
