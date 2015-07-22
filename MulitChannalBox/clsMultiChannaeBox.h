#ifndef CLSMULTICHANNAEBOX_H
#define CLSMULTICHANNAEBOX_H

#include "ui_clsMultiChannaeBox.h"
#include <QString>
#include "clsConnectSWBox.h"
#include <QDebug>

#include "clsMultiModeMeter.h"
#include "clsMRBDisplayPannel.h"


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
    void on_btnSave_clicked();
    void on_btnOpenSettingFile_clicked();
    void on_btnSignleTest_clicked();
    void on_btnSelectChennal_clicked();
    void on_btnClearAllData_clicked();
    void on_btnShowTestStatus_toggled(bool checked);
    void setIdeal();
    void on_btnChannalCal_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *);
private:
    QStringList commands;

    clsMultiModeMeter *meter;

    QString strSaveFileName ;
    QString channels;
    QList<clsMRBDisplayPannel*>pannel;

    void initPannel();
    void initDataBase();
};

#endif // CLSMULTICHANNAEBOX_H
