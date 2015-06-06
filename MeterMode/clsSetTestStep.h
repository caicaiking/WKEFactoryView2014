#ifndef CLSSETTESTSTEP_H
#define CLSSETTESTSTEP_H

#include "ui_clsSetTestStep.h"
#include "WKEMeterMode.h"
class clsSetTestStep : public QDialog, private Ui::clsSetTestStep
{
    Q_OBJECT

public:
    explicit clsSetTestStep(QWidget *parent = 0);

    void setTestSteps(const QStringList &value);

    const QStringList getTestSteps();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();


    void on_btnTurnOffBias_clicked();

    void biasSlot(bool steps);
    void on_btnTest_clicked();

    void showRes(QString steps);


    void on_btnNewStep_clicked();

    void on_btnSave_clicked();


    void on_tbTaskList_clicked(const QModelIndex &index);

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnDelete_clicked();

    void on_btnClear_clicked();

private:
    WKEMeterMode *meter;

private:

    QList<WKEMeterMode *> steps;
    void initSheetTitle();
    QTableWidgetItem *getTableItem(const QString &content, bool isTitle);
    void showTaskList();
};

#endif // CLSSETTESTSTEP_H
