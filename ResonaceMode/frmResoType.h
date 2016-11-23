#ifndef FRMRESOTYPE_H
#define FRMRESOTYPE_H

#include "ui_frmResoType.h"

class frmResoType : public QDialog, private Ui::frmResoType
{
    Q_OBJECT

public:
    explicit frmResoType(QWidget *parent = 0);
    QString getResonanceType();
private slots:

    void on_btnSeries_clicked();

    void on_btnParallel_clicked();

    void on_btnCrystal_clicked();

private:
    QString res;
    bool getResOption();
};

#endif // FRMRESOTYPE_H
