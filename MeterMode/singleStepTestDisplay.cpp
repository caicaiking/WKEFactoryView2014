#include "singleStepTestDisplay.h"

singleStepTestDisplay::singleStepTestDisplay(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void singleStepTestDisplay::setMessage(const QString &msg, int value)
{
    QStringList tmp;
    switch (value) {
    case 0:
        stcWidget->setCurrentIndex(0);
        lblMessage->setText(msg);
        break;
    case 1:
        stcWidget->setCurrentIndex(1);
        tmp = msg.split(",");
        if(tmp.length()==3*1)
        {
            lbl1Par->setText(tmp.at(0));
            lbl1Value->setText(tmp.at(1));
            lbl1Status->showText(tmp.at(2));
        }

        break;
    case 2:
        stcWidget->setCurrentIndex(2);
        tmp = msg.split(",");
        if(tmp.length()==3*2)
        {
            lbl2Par1->setText(tmp.at(0));
            lbl2Value1->setText(tmp.at(1));
            lbl2Status1->showText(tmp.at(2));
            lbl2Par2->setText(tmp.at(3));
            lbl2Value2->setText(tmp.at(4));
            lbl2Status2->showText(tmp.at(5));
        }


        break;
    case 3:
        stcWidget->setCurrentIndex(3);
        tmp = msg.split(",");
        if(tmp.length()==3*3)
        {
            lbl3Par1->setText(tmp.at(0));
            lbl3Value1->setText(tmp.at(1));
            lbl3Status1->showText(tmp.at(2));
            lbl3Par2->setText(tmp.at(3));
            lbl3Value2->setText(tmp.at(4));
            lbl3Status2->showText(tmp.at(5));
            lbl3Par3->setText(tmp.at(6));
            lbl3Value3->setText(tmp.at(7));
            lbl3Status3->showText(tmp.at(8));
        }

        break;
    case 4:
        stcWidget->setCurrentIndex(4);
        tmp = msg.split(",");
        if(tmp.length()==3*4)
        {
            lbl4Par1->setText(tmp.at(0));
            lbl4Value1->setText(tmp.at(1));
            lbl4Status1->showText(tmp.at(2));
            lbl4Par2->setText(tmp.at(3));
            lbl4Value2->setText(tmp.at(4));
            lbl4Status2->showText(tmp.at(5));
            lbl4Par3->setText(tmp.at(6));
            lbl4Value3->setText(tmp.at(7));
            lbl4Status3->showText(tmp.at(8));
            lbl4Par4->setText(tmp.at(9));
            lbl4Value4->setText(tmp.at(10));
            lbl4Status4->showText(tmp.at(11));
        }


        break;
    default:
        break;
    }
}
