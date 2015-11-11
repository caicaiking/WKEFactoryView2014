#ifndef CLSAPPMODE_H
#define CLSAPPMODE_H

#include "ui_clsAppMode.h"
#include "clsMyPushButton.h"
class clsAppMode : public QMainWindow, private Ui::clsAppMode
{
    Q_OBJECT

public:
    explicit clsAppMode(QWidget *parent = 0);
private slots:
    void on_btnCheckNewApp_clicked();
    void addApp(QString appName);
    void btnClick(QString value);
protected:
    void closeEvent(QCloseEvent *);
private:
    QStringList apps;
    QList<clsMyPushButton *> buttons;

private:
    void readSettings();
    void writeSettings();
};

#endif // CLSAPPMODE_H
