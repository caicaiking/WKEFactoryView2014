#ifndef TESTLIMITS_H
#define TESTLIMITS_H

#include "ui_testLimits.h"
#include "clsMeterLimit.h"
#include "frmSetLimit.h"
class testLimits : public QDialog, private Ui::testLimits
{
    Q_OBJECT

public:
    explicit testLimits(QWidget *parent = 0);
private slots:
    void on_btnEditLimits_clicked();

    void setLimit();


private:
    clsMeterLimit lm;
};

#endif // TESTLIMITS_H
