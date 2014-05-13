#ifndef CLSFIT_H
#define CLSFIT_H

#include "ui_clsFit.h"
#include <Plot.h>
class clsFit : public QDialog, private Ui::clsFit
{
    Q_OBJECT
    
public:
    explicit clsFit(QWidget *parent = 0);
    Plot::Choice getChoice(){return choice;}
    void setTraceTitle(QString strTraceA=QString("Z"), QString strTraceB=QString("θ"));
private slots:
    void on_btnBoth_clicked();
    void on_btnTraceA_clicked();
    void on_btnTraceB_clicked();

private:
    Plot::Choice choice;
};

#endif // CLSFIT_H
