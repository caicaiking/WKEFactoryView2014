#include "myKeyBoard.h"
#include <QDate>
#include <QTime>

myKeyBoard::myKeyBoard(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    txtInput->setFocus();
    this->setAutoFillBackground(true);
    Qt::WindowFlags flags=0;
    flags|=Qt::FramelessWindowHint;
    this->setWindowFlags(flags);
    curPos = txtInput->cursorPosition();
    stackedWidget->setCurrentIndex(0);

    this->setGeometry(-2,40,this->width(),this->height());

}

QString myKeyBoard::getInput()
{
    return txtInput->text();
}


void myKeyBoard::addNumber(QString value)
{
    QString strInput = txtInput->text();

    strInput = strInput.insert(curPos,value);
    curPos+=1;
    txtInput->setText(strInput);
    txtInput->setFocus();
    txtInput->setCursorPosition(curPos);
}

void myKeyBoard::on_btn1_clicked()
{
    addNumber("1");
}

void myKeyBoard::on_btn2_clicked()
{
  addNumber("2");

}

void myKeyBoard::on_btn3_clicked()
{
  addNumber("3");
}

void myKeyBoard::on_btn4_clicked()
{
   addNumber("4");
}

void myKeyBoard::on_btn5_clicked()
{
   addNumber("5");
}

void myKeyBoard::on_btn6_clicked()
{
    addNumber("6");
}

void myKeyBoard::on_btn7_clicked()
{
   addNumber("7");
}

void myKeyBoard::on_btn8_clicked()
{
   addNumber("8");
}

void myKeyBoard::on_btn9_clicked()
{
    addNumber("9");
}

void myKeyBoard::on_btn0_clicked()
{
    addNumber("0");
}



void myKeyBoard::on_btnPerc_clicked()
{
    addNumber("%");
}


void myKeyBoard::on_btn__clicked()
{
     addNumber("_");
}

void myKeyBoard::on_btnLeftS_clicked()
{
    addNumber("(");
}

void myKeyBoard::on_btnRightS_clicked()
{
    addNumber(")");
}

void myKeyBoard::on_btnPlush_clicked()
{
    addNumber("+");
}

void myKeyBoard::on_btnFen_clicked()
{
    addNumber(";");
}

void myKeyBoard::on_btnMao_clicked()
{
    addNumber(":");
}

void myKeyBoard::on_btnMinus_clicked()
{
    addNumber("-");
}

void myKeyBoard::on_btnDou_clicked()
{
    addNumber(",");
}

void myKeyBoard::on_btnJu_clicked()
{
     addNumber(".");
}

void myKeyBoard::on_btnGan_clicked()
{
     addNumber("!");
}

void myKeyBoard::on_btnJin_clicked()
{
     addNumber("#");
}

void myKeyBoard::on_btnStar_clicked()
{
     addNumber("*");
}

void myKeyBoard::on_btnLess_clicked()
{
     addNumber("<");
}

void myKeyBoard::on_btnThan_clicked()
{
     addNumber(">");
}



void myKeyBoard::on_btnLeft_clicked()
{
    if(curPos>0)
        curPos-=1;
    txtInput->setCursorPosition(curPos);
    txtInput->setFocus();
}

void myKeyBoard::on_btnRight_clicked()
{
    if(curPos< txtInput->text().length())
        curPos+=1;
    txtInput->setCursorPosition(curPos);
    txtInput->setFocus();
}

void myKeyBoard::on_btnDel_clicked()
{
   txtInput->backspace();

   curPos= txtInput->cursorPosition();
}

void myKeyBoard::on_btnDate_clicked()
{


    QString strToday = QDate::currentDate().toString("yyyyMMdd");

    QString str = txtInput->text();
    str.insert(curPos,strToday);
    curPos+= strToday.length();

    txtInput->setText(str);
    txtInput->setCursorPosition(curPos);
    txtInput->setFocus();

}

void myKeyBoard::on_btnTime_clicked()
{

    QString strTime = QTime::currentTime().toString("hh-mm-ss");
    QString str = txtInput->text();
    str.insert(curPos,strTime);
    curPos+= strTime.length();

    txtInput->setText(str);
    txtInput->setCursorPosition(curPos);
    txtInput->setFocus();
}

void myKeyBoard::changeToLarge()
{
    btnA->setText("A");
    btnB->setText("B");
    btnC->setText("C");
    btnD->setText("D");
    btnE->setText("E");
    btnF->setText("F");
    btnG->setText("G");
    btnH->setText("H");
    btnI->setText("I");
    btnJ->setText("J");
    btnK->setText("K");
    btnL->setText("L");
    btnM->setText("M");
    btnN->setText("N");
    btnO->setText("O");
    btnP->setText("P");
    btnQ->setText("Q");
    btnR->setText("R");
    btnS->setText("S");
    btnT->setText("T");
    btnU->setText("U");
    btnV->setText("V");
    btnW->setText("W");
    btnX->setText("X");
    btnY->setText("Y");
    btnZ->setText("Z");
}

void myKeyBoard::changeToLittle()
{
    btnA->setText("a");
    btnB->setText("b");
    btnC->setText("c");
    btnD->setText("d");
    btnE->setText("e");
    btnF->setText("f");
    btnG->setText("g");
    btnH->setText("h");
    btnI->setText("i");
    btnJ->setText("j");
    btnK->setText("k");
    btnL->setText("l");
    btnM->setText("m");
    btnN->setText("n");
    btnO->setText("o");
    btnP->setText("p");
    btnQ->setText("q");
    btnR->setText("r");
    btnS->setText("s");
    btnT->setText("t");
    btnU->setText("u");
    btnV->setText("v");
    btnW->setText("w");
    btnX->setText("x");
    btnY->setText("y");
    btnZ->setText("z");
}

void myKeyBoard::on_btnLarge_clicked()
{
    if(btnLarge->isChecked())
        changeToLarge();
    else
        changeToLittle();
}





void myKeyBoard::addLetter(QString value)
{
    QString strInput = txtInput->text();

    strInput = strInput.insert(curPos,value);
    curPos+=1;
    txtInput->setText(strInput);
    txtInput->setFocus();
    txtInput->setCursorPosition(curPos);
}

void myKeyBoard::on_btnA_clicked()
{
    addLetter(btnA->text());
}
void myKeyBoard::on_btnB_clicked()
{
    addLetter(btnB->text());
}
void myKeyBoard::on_btnC_clicked()
{
    addLetter(btnC->text());
}
void myKeyBoard::on_btnD_clicked()
{
    addLetter(btnD->text());
}
void myKeyBoard::on_btnE_clicked()
{
    addLetter(btnE->text());
}
void myKeyBoard::on_btnF_clicked()
{
    addLetter(btnF->text());
}
void myKeyBoard::on_btnG_clicked()
{
    addLetter(btnG->text());
}
void myKeyBoard::on_btnH_clicked()
{
    addLetter(btnH->text());
}
void myKeyBoard::on_btnI_clicked()
{
    addLetter(btnI->text());
}
void myKeyBoard::on_btnJ_clicked()
{
    addLetter(btnJ->text());
}
void myKeyBoard::on_btnK_clicked()
{
    addLetter(btnK->text());
}
void myKeyBoard::on_btnL_clicked()
{
    addLetter(btnL->text());
}
void myKeyBoard::on_btnM_clicked()
{
    addLetter(btnM->text());
}
void myKeyBoard::on_btnN_clicked()
{
    addLetter(btnN->text());
}
void myKeyBoard::on_btnO_clicked()
{
    addLetter(btnO->text());
}
void myKeyBoard::on_btnP_clicked()
{
    addLetter(btnP->text());
}
void myKeyBoard::on_btnQ_clicked()
{
    addLetter(btnQ->text());
}
void myKeyBoard::on_btnR_clicked()
{
    addLetter(btnR->text());
}
void myKeyBoard::on_btnS_clicked()
{
    addLetter(btnS->text());
}
void myKeyBoard::on_btnT_clicked()
{
    addLetter(btnT->text());
}
void myKeyBoard::on_btnU_clicked()
{
    addLetter(btnU->text());
}
void myKeyBoard::on_btnV_clicked()
{
    addLetter(btnV->text());
}
void myKeyBoard::on_btnW_clicked()
{
    addLetter(btnW->text());
}
void myKeyBoard::on_btnX_clicked()
{
    addLetter(btnX->text());
}
void myKeyBoard::on_btnY_clicked()
{
    addLetter(btnY->text());
}
void myKeyBoard::on_btnZ_clicked()
{
    addLetter(btnZ->text());
}

void myKeyBoard::on_btnSpace_clicked()
{
    addNumber(" ");
}


void myKeyBoard::on_txtInput_cursorPositionChanged(int /*arg1*/, int arg2)
{
    if(txtInput->hasFocus())
    {
        curPos=arg2;
    }
}

void myKeyBoard::on_btnOk_clicked()
{
    this->accept();
}

void myKeyBoard::on_btnSwitchNumber_clicked()
{
    if(stackedWidget->currentIndex()==0)
    {
        stackedWidget->setCurrentIndex(1);
        btnSwitchNumber->setText("abc");
    }
    else
    {
        stackedWidget->setCurrentIndex(0);
        btnSwitchNumber->setText("123");
    }
}

void myKeyBoard::on_btnQuit_clicked()
{
    this->reject();
}
