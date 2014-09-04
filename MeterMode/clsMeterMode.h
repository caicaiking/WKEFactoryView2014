#ifndef CLSMETERMODE_H
#define CLSMETERMODE_H

#include "ui_clsMeterMode.h"
#include "WKEMeterMode.h"
#include <QList>
struct StepInfo
{
    QString userFunction;
    QString meterFunction;
};

class clsMeterMode : public QMainWindow, private Ui::clsMeterMode
{
    Q_OBJECT

public:
    explicit clsMeterMode(QWidget *parent = 0);
    QTableWidgetItem *getTableItem(const QString &content, bool isTitle=false);
private slots:
    void on_skWidget_currentChanged(int arg1);
    void on_btnRep_clicked();
    void on_btnSetup_clicked();

    void on_btnAdd_clicked();

    void on_btnMeter_clicked();

    void on_btnAddtionFunction_clicked();

    void on_btnTrig_clicked();

    void showList();
protected:
    void closeEvent(QCloseEvent *event);

private:
    QList<WKEMeterMode*> list;
    WKEMeterMode *meter;
    StepInfo stpInfo;

    void setTableTitle();
};

#endif // CLSMETERMODE_H
