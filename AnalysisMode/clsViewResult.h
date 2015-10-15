#ifndef CLSVIEWRESULT_H
#define CLSVIEWRESULT_H
#include <Plot.h>
#include "ui_clsViewResult.h"
#include "Globle.h"
#include "clsComplexOp.h"
class clsViewResult : public QDialog, private Ui::clsViewResult
{
    Q_OBJECT
    
public:
    explicit clsViewResult(const  QList<PlotCurves> value,const QStringList Headers,QWidget *parent = 0);
    void setCurves(const  QList<PlotCurves> &value);
    void setLabels(QString sweepItems, QString strTraceA, QString strTraceB);

    void setParameters(QString item1, QString item2,QString eqcct,SweepType sweepType);
    QList<double> getValues(double z, double a, double freq);
private slots:
    void on_btnCancel_clicked();
    void on_btnSave_clicked();

    void showCurve(int intSelect);
    void formateDataShow();
    void on_tvResult_doubleClicked(const QModelIndex &index);



signals:
    void setMark(const double value,const int curveSelected);
private:
     QList<PlotCurves> curves;

     QString strSweepItems;
     QString strTraceA;
     QString strTraceB;


     QMap <double, QPointF> data;
     void saveExcel(QString fileName);
     void saveCsv(QString fileName);

     QString item1;
     QString item2;
     Equcct equcct;
     SweepType sweepType;

     QStringList saveValues;
     void setDemoVersion(bool value);
};

#endif // CLSVIEWRESULT_H
