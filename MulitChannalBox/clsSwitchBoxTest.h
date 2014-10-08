#ifndef CLSSWITCHBOXTEST_H
#define CLSSWITCHBOXTEST_H

#include "ui_clsSwitchBoxTest.h"

class clsSwitchBoxTest : public QDialog, private Ui::clsSwitchBoxTest
{
    Q_OBJECT

public:
    explicit clsSwitchBoxTest(QWidget *parent = 0);
private slots:
    void on_btnLoop_clicked();
    void on_txtChannel_valueChanged(int arg1);

private:
    QStringList commands;
};

#endif // CLSSWITCHBOXTEST_H
