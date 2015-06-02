#ifndef CLSSETTESTSTEP_H
#define CLSSETTESTSTEP_H

#include "ui_clsSetTestStep.h"
#include "WKEMeterMode.h"
class clsSetTestStep : public QDialog, private Ui::clsSetTestStep
{
    Q_OBJECT

public:
    explicit clsSetTestStep(QWidget *parent = 0);

    void setTestSteps(QList<WKEMeterMode *> value);

    QList<WKEMeterMode *> getTestSteps();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();


    void on_btnTurnOffBias_clicked();

    void biasSlot(bool value);
    void on_btnTest_clicked();

    void showRes(QString value);


    void on_btnNewStep_clicked();

    void on_btnSave_clicked();

private:
    WKEMeterMode *meter;

private:
    QString step;
};

#endif // CLSSETTESTSTEP_H
