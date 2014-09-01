#ifndef CLSMETERMODE_H
#define CLSMETERMODE_H

#include "ui_clsMeterMode.h"

class clsMeterMode : public QMainWindow, private Ui::clsMeterMode
{
    Q_OBJECT

public:
    explicit clsMeterMode(QWidget *parent = 0);
private slots:
    void on_skWidget_currentChanged(int arg1);
    void on_btnRep_clicked();
    void on_btnSetup_clicked();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CLSMETERMODE_H
