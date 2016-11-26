#ifndef CLSSWITCHBOXTEST_H
#define CLSSWITCHBOXTEST_H

#include "ui_clsSwitchBoxTest.h"

class clsSwitchBoxTest : public QDialog, private Ui::clsSwitchBoxTest
{
    Q_OBJECT

public:
    explicit clsSwitchBoxTest(QWidget *parent = 0);
    void setTerminal(int value);

private slots:
    void on_btnLoop_clicked();
    void on_txtChannel_valueChanged(int arg1);

    void on_btnChangeTerminal_clicked();

    void on_btnChangeTerminal4_clicked();

    void on_btnSetPass_clicked();

    void on_btnSetFail_clicked();

    void on_btnSendCommand_clicked();


    void on_btnSetIdel_clicked();

private:
    QStringList commands;
    int terminal;

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // CLSSWITCHBOXTEST_H
