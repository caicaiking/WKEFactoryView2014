#ifndef CLSMULTICHANNAEBOX_H
#define CLSMULTICHANNAEBOX_H

#include "ui_clsMultiChannaeBox.h"
#include <QString>
#include "clsConnectSWBox.h"
#include <QDebug>
#include "WKEMeterMode.h"
class clsMultiChannaeBox : public QMainWindow, private Ui::clsMultiChannaeBox
{
    Q_OBJECT

public:
    explicit clsMultiChannaeBox(QWidget *parent = 0);
    static QStringList initCommand();
private slots:

    void on_btnOpen_clicked();
    void on_btnSwitchBoxTest_clicked();


    void on_btnMeter_clicked();

private:
    QStringList commands;

    WKEMeterMode *meter;




};

#endif // CLSMULTICHANNAEBOX_H
