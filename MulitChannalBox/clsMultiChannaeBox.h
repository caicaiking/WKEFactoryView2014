#ifndef CLSMULTICHANNAEBOX_H
#define CLSMULTICHANNAEBOX_H

#include "ui_clsMultiChannaeBox.h"
#include <QString>
#include "clsConnectSWBox.h"
#include <QDebug>
#include "WKEMeterMode.h"
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

    void on_btnSelectChennal_2_clicked();

private:
    QStringList commands;
    WKEMeterMode *meter;
    QString strSaveFileName ;

    QString chennal;

    QString strResult;

    clsMRBDisplayPannel *pannel;


};

#endif // CLSMULTICHANNAEBOX_H
