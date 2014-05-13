#ifndef FRMPOINTEDITOR_H
#define FRMPOINTEDITOR_H

#include "ui_frmPointEditor.h"
#include "Globle.h"
#include <QList>

class frmPointEditor : public QDialog, private Ui::frmPointEditor
{
    Q_OBJECT

public:
    explicit frmPointEditor(const double min, const double max,
                            const bool blLog, const SweepType t, QWidget *parent = 0);

    void setPoints(QList<double> points);
    void setSweepType(SweepType t);
    void setMaxAndMinValue(double max, double min);

    static QList<double> getStaticPoints(const double min, const double max,
                                         const bool blLog, /*const SweepType t,*/ int numPoints);
    static QList<double> rmvPP(QList<double> points);
    QList<double>  getPoints()
    {


        return points;
    }
public slots:
    void setTestPoint(int value);
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btn10_clicked();
    void on_btn20_clicked();
    void on_btn50_clicked();
    void on_btn100_clicked();
    void on_btn200_clicked();
    void on_btn400_clicked();
    void on_btn800_clicked();
    void on_btn1200_clicked();
    void on_btn1600_clicked();

    void on_btnCustom_clicked();
    void on_btnNewPoint_clicked();
    void on_btnSort_clicked();
    void on_btnDel_clicked();

private:
    double min;
    double max;
    bool blLog;
    SweepType t;

    QList<double> points;

    QTableWidgetItem *getItem(QString value);
    void setTable();
    QTableWidgetItem *getContentItem(QString value);
    void showTestData();

};

#endif // FRMPOINTEDITOR_H
