#include "clsSelectedRef.h"

clsSelectedRef::clsSelectedRef(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    connect(this,SIGNAL(selected(int)),this,SLOT(setSelected(int)));
}



void clsSelectedRef::on_btn1_clicked()
{
    emit selected(1);
}

void clsSelectedRef::on_btn2_clicked()
{
     emit selected(2);
}

void clsSelectedRef::on_btn3_clicked()
{
     emit selected(3);
}

void clsSelectedRef::on_btn4_clicked()
{
     emit selected(4);
}

void clsSelectedRef::on_btn5_clicked()
{
     emit selected(5);
}

void clsSelectedRef::on_btn6_clicked()
{
     emit selected(6);
}

void clsSelectedRef::on_btn7_clicked()
{
     emit selected(7);
}

void clsSelectedRef::on_btn8_clicked()
{
     emit selected(8);
}

void clsSelectedRef::on_btn9_clicked()
{
     emit selected(9);
}

void clsSelectedRef::on_btn10_clicked()
{
     emit selected(10);
}

//void clsSelectedRef::on_btn11_clicked()
//{
//     emit selected(11);
//}


//void clsSelectedRef::on_btn12_clicked()
//{
//     emit selected(12);
//}

//void clsSelectedRef::on_btn13_clicked()
//{
//     emit selected(13);
//}

//void clsSelectedRef::on_btn14_clicked()
//{
//     emit selected(14);
//}

//void clsSelectedRef::on_btn15_clicked()
//{
//     emit selected(15);
//}


//void clsSelectedRef::on_btn16_clicked()
//{
//     emit selected(16);
//}

void clsSelectedRef::setSelected(int intSelected)
{
    this->intSelected= intSelected;
    this->accept();
}




void clsSelectedRef::on_btnCancel_clicked()
{
    this->reject();
}

void clsSelectedRef::on_btnDefault_clicked()
{
  emit selected(0);
}
