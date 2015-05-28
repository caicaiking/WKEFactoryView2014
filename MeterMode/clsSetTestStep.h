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

private:
    WKEMeterMode *meter;
};

#endif // CLSSETTESTSTEP_H
