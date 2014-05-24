#ifndef FRMPEAKSEARCH_H
#define FRMPEAKSEARCH_H

#include "ui_frmPeakSearch.h"
#include "Plot.h"
#include "Globle.h"
class frmPeakSearch : public QDialog, private Ui::frmPeakSearch
{
    Q_OBJECT

public:
    explicit frmPeakSearch(Plot* plot, const graphSetup gs, QWidget *parent = 0);

    void setPlot(Plot *plot);
    void setGraphSetup(const graphSetup gs);
private slots:


    void on_btnExit_clicked();



    void on_btnPeak_toggled(bool checked);

    void on_btnItem1_toggled(bool checked);

    void on_btnItem2_toggled(bool checked);

    void on_btnDip_toggled(bool checked);

    void on_btnFindPeak_clicked();

    void on_btnFindDip_clicked();

    void on_btnLeft_clicked();

    void on_btnRight_clicked();

private:
    Plot *plot;
    graphSetup gs;

    Plot::Choice choice;
};

#endif // FRMPEAKSEARCH_H
