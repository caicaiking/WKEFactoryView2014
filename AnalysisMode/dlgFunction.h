#ifndef DLGFUNCTION_H
#define DLGFUNCTION_H

#include "ui_dlgFunction.h"

class dlgFunction : public QDialog, private Ui::dlgFunction
{
    Q_OBJECT
    
public:
    explicit dlgFunction(bool btnOffVisible=true, bool btnRdcVisible=true, QWidget *parent = 0);
//    void setOffVisiable(bool value)
//    {
//        this->btnOFF->setVisible(value);
//    }

    QString getItem()
    {
        return this->strItem;
    }

    void setMateralFunction(bool value);

private slots:
    void on_btnC_clicked();
    void on_btnD_clicked();
    void on_btnL_clicked();
    void on_btnQ_clicked();
    void on_btnR_clicked();
    void on_btnX_clicked();
    void on_btnG_clicked();
    void on_btnB_clicked();
    void on_btnZ_clicked();
    void on_btnA_clicked();
    void on_btnY_clicked();
//    void on_btnRdc_clicked();
    void on_btnCancel_clicked();
//    void on_btnOFF_clicked();



    void on_btnE1R_clicked();

    void on_btnE2R_clicked();

    void on_btnDe_clicked();

    void on_btnU1R_clicked();

    void on_btnU2R_clicked();

    void on_btnDu_clicked();

private:
    QString strItem;
};

#endif // DLGFUNCTION_H
