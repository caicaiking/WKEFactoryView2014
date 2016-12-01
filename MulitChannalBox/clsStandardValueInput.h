#ifndef CLSSTANDARDVALUEINPUT_H
#define CLSSTANDARDVALUEINPUT_H

#include "ui_clsStandardValueInput.h"

class clsStandardValueInput : public QDialog, private Ui::clsStandardValueInput
{
    Q_OBJECT

public:
    explicit clsStandardValueInput(QWidget *parent = 0);
    void setFrequency(double value);
    double getZ() const;
    double getA() const;
    void setZ(double value);
    void setA(double value);


private slots:
    void on_cmdItems_currentIndexChanged(int index);
    void on_btnItem1_clicked();

    void on_btnItem2_clicked();

    void on_btnOk_clicked();

    void on_btnEqucct_clicked();

    void on_btnCancel_clicked();

private:
    QString item1,item2;
    QString equcct;
    double z;
    double a;
    double frequency;
    double dblItem1,dblItem2;
    void updateText();
    void convertRpXpToZA(double rp, double xp);
    void convertRsXsToZA(double Rs, double Xs);

    const double PI = 3.14159265359	;
};

#endif // CLSSTANDARDVALUEINPUT_H
