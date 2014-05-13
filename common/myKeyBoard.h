#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include "ui_myKeyBoard.h"

class myKeyBoard : public QDialog, private Ui::myKeyBoard
{
    Q_OBJECT
    
public:
    explicit myKeyBoard(QWidget *parent = 0);

    QString getInput();
    void setInput(QString value)
    {
        txtInput->setText(value);
        curPos = value.length();
        txtInput->setCursorPosition(curPos);
    }

private slots:
    void on_btn0_clicked();
    void on_btn9_clicked();
    void on_btn8_clicked();
    void on_btn7_clicked();
    void on_btn6_clicked();
    void on_btn5_clicked();
    void on_btn4_clicked();
    void on_btn3_clicked();
    void on_btn2_clicked();
    void on_btn1_clicked();
    void on_btnPerc_clicked();
    void on_btn__clicked();
    void on_btnLeftS_clicked();
    void on_btnRightS_clicked();
    void on_btnPlush_clicked();
    void on_btnFen_clicked();
    void on_btnMao_clicked();
    void on_btnMinus_clicked();
    void on_btnDou_clicked();
    void on_btnJu_clicked();
    void on_btnGan_clicked();
    void on_btnJin_clicked();
    void on_btnStar_clicked();
    void on_btnLess_clicked();
    void on_btnThan_clicked();

    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_btnDel_clicked();
    void on_btnDate_clicked();
    void on_btnTime_clicked();
    void on_btnLarge_clicked();


    void on_btnA_clicked();
    void on_btnB_clicked();
    void on_btnC_clicked();
    void on_btnD_clicked();
    void on_btnE_clicked();
    void on_btnF_clicked();
    void on_btnG_clicked();
    void on_btnH_clicked();
    void on_btnI_clicked();
    void on_btnJ_clicked();
    void on_btnK_clicked();
    void on_btnL_clicked();
    void on_btnM_clicked();
    void on_btnN_clicked();
    void on_btnO_clicked();
    void on_btnP_clicked();
    void on_btnQ_clicked();
    void on_btnR_clicked();
    void on_btnS_clicked();
    void on_btnT_clicked();
    void on_btnU_clicked();
    void on_btnV_clicked();
    void on_btnW_clicked();
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();

    void on_btnSpace_clicked();

    void on_txtInput_cursorPositionChanged(int , int arg2);
    void on_btnOk_clicked();


    void addNumber(QString value);
    void changeToLarge();
    void changeToLittle();
    void addLetter(QString value);



    void on_btnSwitchNumber_clicked();

    void on_btnQuit_clicked();

private:
    int curPos;
};

#endif // MYKEYBOARD_H
