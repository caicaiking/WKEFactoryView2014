#ifndef clsMultModeMeterUi_H
#define clsMultModeMeterUi_H

#include "ui_clsMultModeMeterUi.h"

#include "clsMeterLimit.h"
#include "NumberInput.h"
#include <QJsonDocument>
#include "clsMultiModeMeter.h"

class clsMultiModeMeterUi : public QDialog, private Ui::clsMultModeMeterUi
{
    Q_OBJECT

public:
    explicit clsMultiModeMeterUi(QWidget *parent = 0);
    void setConditon(QString value);
    QString getConditon();

private slots:
    void setTest1Limit1();
    void setTest2Limit1();
    void setTest1Limit2();
    void setTest2Limit2();

    void on_btnTest1Frequency_clicked();
    void on_btnTest2Frequency_clicked();
    void on_btnTest1Level_clicked();
    void on_btnTest2Level_clicked();
    void on_btnTest1Range_clicked();
    void on_btnTest1Speed_clicked();
    void on_btnTest1Equcct_clicked();
    void on_btnTest2Equcct_clicked();
    void on_btnTest1Item1_clicked();
    void on_btnTest1Item2_clicked();
    void on_btnTest2Item1_clicked();
    void on_btnTest2Item2_clicked();
    void on_btnOk_clicked();
    void on_grpTest1Item2_toggled(bool arg1);
    void on_grpTest2_toggled(bool arg1);
    void on_grpTest2Item2_toggled(bool arg1);
    void on_btnTest1Item1Unit_clicked();
    void on_btnTest1Item2Unit_clicked();
    void on_btnTest2Item1Unit_clicked();
    void on_btnTest2Item2Unit_clicked();

private:
    clsMultiModeMeter *meter;

    void updataButtons();
};

#endif // clsMultModeMeterUi_H
