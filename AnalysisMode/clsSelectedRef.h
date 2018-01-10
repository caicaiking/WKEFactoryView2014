#ifndef CLSSELECTEDREF_H
#define CLSSELECTEDREF_H

#include "ui_clsSelectedRef.h"

class clsSelectedRef : public QDialog, private Ui::clsSelectedRef
{
    Q_OBJECT
    
public:
    explicit clsSelectedRef(QWidget *parent = 0);
    int getSelected(){return this->intSelected;}
signals:
    void selected(int intSelected);
private slots:

    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();
    void on_btn4_clicked();
    void on_btn5_clicked();
    void on_btn6_clicked();
    void on_btn7_clicked();
    void on_btn8_clicked();
    void on_btn9_clicked();
    void on_btn10_clicked();
//    void on_btn11_clicked();
//    void on_btn12_clicked();
//    void on_btn13_clicked();
//    void on_btn14_clicked();
//    void on_btn15_clicked();
//    void on_btn16_clicked();

    void setSelected(int intSelected);

    void on_btnCancel_clicked();

    void on_btnDefault_clicked();

private:
    int intSelected;
};

#endif // CLSSELECTEDREF_H
