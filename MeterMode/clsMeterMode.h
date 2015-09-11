#ifndef CLSMETERMODE_H
#define CLSMETERMODE_H

#include "ui_clsMeterMode.h"
#include "WKEMeterMode.h"
#include <QList>
#include <QPointer>
#include "UserfulFunctions.h"
#include "clsSignalThread.h"
#include "clsTestResult.h"
/*!
 * 用于产品的统计
 * !*/
class product_Static
{
public:
    int totle;
    int pass;
    int fail;
public:
    product_Static(){
        totle=1;
        pass =0;
        fail=0;
    }

    QString getTotle()
    {
        return QString::number(totle);
    }

    QString getPass()
    {
        return QString::number(pass);
    }

    QString getFail()
    {
        return QString::number(fail);
    }

    void reset()
    {
        totle=1;
        pass =0;
        fail=0;
    }
    void increase(bool isPass)
    {
        totle++;
        if(isPass)
            pass++;
        else
            fail++;
    }
};


/*!
 *
 * !*/


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
    void on_btnRep_clicked();
    void on_btnStop_clicked();
    void on_btnNewTask_clicked();
    void detecInprogress();
    void on_btnSaveData_clicked();
    void on_btnAdvance_clicked();
    void trig();
    void setAdu200(Status value);
    void on_btnStartDetect_clicked();
    void showMessage(QString value);
    void on_btnReport_clicked();

    void adu200Trig();

    void on_btnRep10_clicked();
protected:
    void closeEvent(QCloseEvent *);
private:
    QList<WKEMeterMode *> steps;
    WKEMeterMode *meter;
    product_Static count;
    bool isStop;
    bool blSingleDisplay;
    void initTable();
    QTableWidgetItem *getTableTitleItem(const QString &content);
    QTableWidgetItem *getTableTestItem(const QString &content, int color);
    QString strTaskFile;
    QString strDataFile;
    QString tmpDir;
    void readSettings();
    void saveSettings();
    MeterSettings mSettings;
    void updateMessage();
    clsSignalThread * adu200;
    void saveDataFile(QString value);

    clsTestResult result;   //用于记录保存数据
};

#endif // CLSMETERMODE_H
