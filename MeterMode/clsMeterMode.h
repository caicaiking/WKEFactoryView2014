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

private slots:
    void on_btnSetStep_clicked();
    void on_btnSaveTask_clicked();
    void on_btnOpenTask_clicked();
    void on_btnTrig_clicked();
    void on_btnCalibration_clicked();

protected:

private:
    QList<WKEMeterMode *> steps;
    WKEMeterMode *meter;

    int count;


    void trig();
    void initTable();
    QTableWidgetItem *getTableTitleItem(const QString &content);
    QTableWidgetItem *getTableTestItem(const QString &content, int color);
};

#endif // CLSMETERMODE_H
