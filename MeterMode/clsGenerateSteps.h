#ifndef CLSGENERATESTEPS_H
#define CLSGENERATESTEPS_H

#include "ui_clsGenerateSteps.h"
#include "Globle.h"

class clsGenerateSteps : public QDialog, private Ui::clsGenerateSteps
{
    Q_OBJECT

public:
    explicit clsGenerateSteps(QWidget *parent = 0);
    SweepType getType();
    QList<double> getPoints() const;
protected:
private slots:
    void on_btnFrequency_clicked();
    void on_btnBiasA_clicked();
    void on_btnLevelA_clicked();
    void on_btnLevelV_clicked();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnPoint_clicked();
    void on_btnGenerate_clicked();
    void on_btnOpenDataFile_clicked();
    void on_chkLog_toggled(bool checked);

private:
    SweepType changeValue;
    double start;
    double stop;
    bool log;
    int point;
    QList<double> points;

    void updateButtons();
    void showPoints();
};

#endif // CLSGENERATESTEPS_H
