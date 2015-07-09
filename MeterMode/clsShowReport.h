#ifndef CLSSHOWREPORT_H
#define CLSSHOWREPORT_H

#include "ui_clsShowReport.h"
#include "clsDataStore.h"
class clsShowReport : public QDialog, private Ui::clsShowReport
{
    Q_OBJECT

public:
    explicit clsShowReport(QWidget *parent = 0);

    void setData(clsDataStore *value);

private slots:
    void on_btnShow_clicked();

    void on_txtStep_valueChanged(int arg1);

private:
    clsDataStore *result;
};

#endif // CLSSHOWREPORT_H
