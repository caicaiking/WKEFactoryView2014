#ifndef CLSSTATISTICS_H
#define CLSSTATISTICS_H

#include "ui_clsStatistics.h"
#include "clsDataStore.h"
class clsStatistics : public QDialog, private Ui::clsStatistics
{
    Q_OBJECT

public:
    explicit clsStatistics(QWidget *parent = 0);

    void setData(clsDataStore *value);

private slots:
    void on_txtStep_valueChanged(int arg1);
    void on_txtItem_valueChanged(int arg1);
    void on_btnDisplay_clicked();
    void on_btnExport_clicked();
    
private:
    clsDataStore * result;

private:
    void updateButtons();
};

#endif // CLSSTATISTICS_H
