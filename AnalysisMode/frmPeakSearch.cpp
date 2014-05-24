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
    this->btnFindDip->setVisible(false);

    btnItem1->setEnabled(plot->getYLeftIsEnable());
    btnItem2->setEnabled(plot->getYRightIsEnable());

    if(btnItem2->isEnabled())
        btnItem2->setChecked(true);

    if(btnItem1->isEnabled())
        btnItem1->setChecked(true);

}

void frmPeakSearch::setPlot(Plot *plot)
{
    this->plot = plot;
}

void frmPeakSearch::setGraphSetup(const graphSetup gs)
{
    this->gs=gs;
}

void frmPeakSearch::on_btnExit_clicked()
{
    this->accept();
}


void frmPeakSearch::on_btnPeak_toggled(bool checked)
{
    btnDip->setChecked(!checked);


    btnFindPeak->setVisible(checked);
    btnFindDip->setVisible(!checked);

}

void frmPeakSearch::on_btnItem1_toggled(bool checked)
{
    btnItem2->setChecked(!checked);
}

void frmPeakSearch::on_btnItem2_toggled(bool checked)
{
    btnItem1->setChecked(!checked);
}

void frmPeakSearch::on_btnDip_toggled(bool checked)
{
    btnPeak->setChecked(!checked);
    btnFindPeak->setVisible(!checked);
    btnFindDip->setVisible(checked);
}

void frmPeakSearch::on_btnFindPeak_clicked()
{
    if(btnItem1->isChecked())
        choice = Plot::yLeft;
    else if(btnItem2->isChecked())
        choice = Plot::yRight;
    else
        return;

    plot->findPeak(choice,true);
}

void frmPeakSearch::on_btnFindDip_clicked()
{
    if(btnItem1->isChecked())
        choice = Plot::yLeft;
    else if(btnItem2->isChecked())
        choice = Plot::yRight;
    else
        return;

    plot->findPeak(choice,false);
}

void frmPeakSearch::on_btnLeft_clicked()
{
    if(btnItem1->isChecked())
        choice = Plot::yLeft;
    else if(btnItem2->isChecked())
        choice = Plot::yRight;
    else
        return;

    if(btnPeak->isChecked())
        plot->findNextLeftHigh(choice);
    else
        plot->findNextLeftLow(choice);
}

void frmPeakSearch::on_btnRight_clicked()
{
    if(btnItem1->isChecked())
        choice = Plot::yLeft;
    else if(btnItem2->isChecked())
        choice = Plot::yRight;
    else
        return;

    if(btnPeak->isChecked())
        plot->findNextRightHigh(choice);
    else
        plot->findNextRightLow(choice);

}
