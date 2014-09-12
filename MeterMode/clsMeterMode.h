#ifndef CLSMETERMODE_H
#define CLSMETERMODE_H

#include "ui_clsMeterMode.h"
#include "WKEMeterMode.h"
#include <QList>
#include <QPointer>


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
    void showTaskList();
    void on_btnDelete_clicked();
    void on_btnUp_clicked();
    void on_btnDown_clicked();

    void on_tbTaskList_clicked(const QModelIndex &index);

    void on_btnSaveStep_clicked();

    void enableBtnSaveStep();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QList<QPointer<WKEMeterMode> > list;
    QPointer<WKEMeterMode> meter;
    void setTableTitle();
};

#endif // CLSMETERMODE_H
