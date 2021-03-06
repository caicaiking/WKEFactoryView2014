#ifndef CLSSHOWREPORT_H
#define CLSSHOWREPORT_H

#include "ui_clsShowReport.h"
#include "clsDataStore.h"
#include <QSqlError>
class clsShowReport : public QDialog, private Ui::clsShowReport
{
    Q_OBJECT

public:
    explicit clsShowReport(QWidget *parent = 0);

    void setData(clsDataStore *value);

private slots:

    void on_btnClose_clicked();

    void on_btnExport_clicked();

private:
    clsDataStore *result;
    QString tmpDir;
    QString strLevel;
private:
    void readSettings();
    void saveSettings();
    void updateButtons();
    void printError(QSqlError error);
    void addCmbReport();//添加了多个报表导出
};

#endif // CLSSHOWREPORT_H
