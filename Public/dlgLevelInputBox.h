#ifndef dlgLEVELINPUTBOX_H
#define dlgLEVELINPUTBOX_H

#include "ui_dlgLevelInputBox.h"

class dlgLevelInputBox : public QDialog, private Ui::dlgLevelInputBox
{
    Q_OBJECT
    
public:
    explicit dlgLevelInputBox(QWidget *parent = 0);
 explicit dlgLevelInputBox(const double,const QString,QWidget *parent = 0);
    void setValueAndSuffix(const double,const QString);
    double getValue(){ return  value;}
    QString getSuffix(){return  suffix;}
    QString getUnit(){return  unit;}
    void setAVisible(bool value);

private slots:
    void on_btnClear_clicked();
    void on_btnV_clicked();

    void on_btnM_clicked();
    void on_btnk_clicked();
    void on_btnm_clicked();
    void on_btnu_clicked();



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

#endif // dlgLEVELINPUTBOX_H
