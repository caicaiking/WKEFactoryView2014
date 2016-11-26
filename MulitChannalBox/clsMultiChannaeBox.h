void on_btnSwitchTerminal_clicked();
#ifndef CLSMULTICHANNAEBOX_H
#define CLSMULTICHANNAEBOX_H

#include "ui_clsMultiChannaeBox.h"
#include <QString>
#include "clsConnectSWBox.h"
#include <QDebug>

#include <QCheckBox>
#include "clsMultiModeMeter.h"
#include "clsMRBDisplayPannel.h"

#include "multiModePlot.h"

class clsMultiChannaeBox : public QMainWindow, private Ui::clsMultiChannaeBox
{
    Q_OBJECT

public:
    explicit clsMultiChannaeBox(QWidget *parent = 0);

    void itemTrig(clsMRBDisplayPannel *value);

    void initTestTable();
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

    void on_btnRunningSettings_clicked();

    void itemClick(clsMRBDisplayPannel *);
    void on_btnSelectMode_clicked();

    void setCurveEnable(bool);

    void on_cmbItem_currentIndexChanged(int index);


    void on_btnOpenDataDir_clicked();

    void on_btnAboutMe_clicked();

    void on_btnSavePic_clicked();
    void on_btnSwitchTerminal_clicked();

protected: void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *);
private:
    QStringList commands;
    clsMultiModeMeter *meter;
    QString strSaveFileName ;
    QString channels;
    QList<clsMRBDisplayPannel*>pannel;

    QList<QCheckBox*> checkBoxs;

    int terminal;
    bool isFastGpibOn;
    bool isLedOff;

    void initPannel();
    void initSweepMode();
    void initDataBase();
    void stop();

    void readSettings();
    void writeSettings();

    int switchDelay;
    bool isUseLoadValue;

    void mkDataDir();
    void setTerminal(int value);
};

#endif // CLSMULTICHANNAEBOX_H
