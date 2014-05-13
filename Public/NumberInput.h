#ifndef NUMBERINPUT_H
#define NUMBERINPUT_H
#include "doubleType.h"
#include "ui_NumberInput.h"

class NumberInput : public QDialog, private Ui::NumberInput
{
    Q_OBJECT
    
public:
    explicit NumberInput(QWidget *parent = 0);
    explicit NumberInput(const double,const QString,QWidget *parent = 0);
    void setValueAndSuffix(const double,const QString);
    double getValue(){ return  value;}
    QString getSuffix(){return  suffix;}

    double getNumber()
    {
        doubleType dt(value, suffix);
        return dt.Data();
    }


private slots:
    void on_btnClear_clicked();

    void on_btnG_clicked();
    void on_btnM_clicked();
    void on_btnk_clicked();
    void on_btnm_clicked();
    void on_btnu_clicked();
    void on_btnn_clicked();
    void on_btnp_clicked();
    void on_btnf_clicked();


    void on_btnCancel_clicked();

    void numberInput(int i);
    void signInput();

    void on_btnSign_clicked();

    void on_btn7_clicked();

    void on_btn8_clicked();

    void on_btn9_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn6_clicked();

    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btn0_clicked();

    void on_btnDota_clicked();

    void on_btnDel_clicked();

    void on_btnOk_clicked();

private:

    double value;
    QString suffix;
    bool stopInput;
    bool firstInput;

    QString strInput;
};

#endif // NUMBERINPUT_H
