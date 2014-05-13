#ifndef WK6500LEVELINPUTBOX_H
#define WK6500LEVELINPUTBOX_H

#include "ui_wk6500LevelInputBox.h"

class wk6500LevelInputBox : public QDialog, private Ui::wk6500LevelInputBox
{
    Q_OBJECT
    
public:
    explicit wk6500LevelInputBox(QWidget *parent = 0);
 explicit wk6500LevelInputBox(const double,const QString,QWidget *parent = 0);
    void setValueAndSuffix(const double,const QString);
    double getValue(){ return  value;}
    QString getSuffix(){return  suffix;}
    QString getUnit(){return  unit;}

    void setAmpEnable(bool status=true);
private slots:
    void on_btnClear_clicked();
    void on_btnV_clicked();
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
    void signInput(QString);

    void on_btnA_clicked();

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

private:

    double value;
    QString suffix;
    bool stopInput;
    bool firstInput;
    QString unit;

    QString strInput;

};

#endif // WK6500LEVELINPUTBOX_H
