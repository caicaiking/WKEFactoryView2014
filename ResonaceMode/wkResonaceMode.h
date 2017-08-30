#ifndef WKRESONACEMODE_H
#define WKRESONACEMODE_H

#include "ui_wkResonaceMode.h"
#include "clsResonaceMode.h"
#include "clsMeterLimit.h"
#include "frmSetLimit.h"
#include "clsSignalThread.h"
#include <QMainWindow>
class wkResonaceMode : public QMainWindow, private Ui::wkResonaceMode
{
    Q_OBJECT

public:
    explicit wkResonaceMode(QWidget *parent = 0);

private slots:

    void on_chkCp_toggled(bool checked);
    void on_btnEqucct_clicked();
    void on_btnSpeed_clicked();
    void resTypeChanged(const QString &value);
    void on_btnDepth_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnPreSearch_clicked();
    void on_btnSearch_clicked();
    void setLimit();
    void setLimitFafr();
    void on_btnShowSetup_clicked();
    void threadInfo();
    void trig();
    void on_btnExit_clicked();
    void on_btnSaveDataFile_clicked();
    void writeFileHead();
    void writeData(double freq, double c, double l, double r, double q, bool status = true);
    void writeData(double r, double c, double l, double C0, double q, double keff, double cp1,
                   double fr, double fa, double fm, double fn, double fs, double fp, double fafr, bool status);
    void on_btnInfo_clicked();

    void on_chkCp_clicked();

private:
    QString resEqucct;
    QString resSpeed;
    int resDepth;
    double resStart;
    double resStop;
    bool cpChecked;
    void updateBtn();
    clsResonaceMode *resMode;
    clsMeterLimit m1,m2;
    bool status;
    clsSignalThread *thread;
    QString strFilePath;

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // WKRESONACEMODE_H
