#ifndef CLSBISAOPERATION_H
#define CLSBISAOPERATION_H

#include "ui_clsBiasAOperation.h"

class clsBiasAOperation : public QDialog, private Ui::clsBiasAOperation
{
    Q_OBJECT

public:
    explicit clsBiasAOperation(QWidget *parent = 0);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_btnReset_clicked();

private:
    bool blPreBias, blAfterBias;
    int preValue, afterValue;
    void updateButtion();
    void writeSettings();
    void readSettings();
};

#endif // CLSBISAOPERATION_H
