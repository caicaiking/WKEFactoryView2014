#include "dlgLevelInputBox.h"

dlgLevelInputBox::dlgLevelInputBox(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    stopInput=false;
}

dlgLevelInputBox::dlgLevelInputBox(const double value, const QString suffix, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);
    this->value =value;
    this->suffix = suffix;
    firstInput = false;
    stopInput = false;
    strInput =QString::number(value);

    txtNumber->setText(QString::number(value)+suffix);
}

void dlgLevelInputBox::setValueAndSuffix(const double value, const QString suffix)
{
    this->value =value;
    this->suffix = suffix;
    firstInput = false;
    stopInput = false;
    strInput =QString::number(value);
    txtNumber->setText(QString::number(value)+suffix);
}

void dlgLevelInputBox::setAVisible(bool value)
{
    btnA->setVisible(value);
}

void dlgLevelInputBox::on_btnClear_clicked()
{

    txtNumber->clear();
    strInput ="";
    this->value=0;
    this->suffix="";
    firstInput = false;
    stopInput = false;
}

void dlgLevelInputBox::on_btnV_clicked()
{
    this->unit="V";
    this->accept();

}

void dlgLevelInputBox::on_btnM_clicked()
{
    if (!strInput.isEmpty() && (!stopInput))
    {

        this->suffix=("M");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void dlgLevelInputBox::on_btnk_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("k");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void dlgLevelInputBox::on_btnm_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("m");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void dlgLevelInputBox::on_btnu_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("u");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}





void dlgLevelInputBox::on_btnCancel_clicked()
{
    this->reject();
}

void dlgLevelInputBox::numberInput(int i)
{
    if(!firstInput)
    {

        on_btnClear_clicked();
        firstInput = true;
    }

    if(stopInput)
        return;

    strInput +=QString::number(i);

    bool ok;
    double dbl = strInput.toDouble(&ok);
    if (ok && dbl !=0.0 )
    {
        txtNumber->setText(strInput);
        this->value = dbl;
    }
    else
    {
        strInput="";
        txtNumber->setText("0");
        this->value =0.0;
    }



}

void dlgLevelInputBox::signInput(QString)
{
    if(strInput.contains("-"))
    {
        strInput.remove("-");
    }
    else
    {
        strInput="-"+ strInput;
    }

    bool ok;
    double dbl=strInput.toDouble(&ok);

    if(ok)
    {
        this->value =dbl;
        txtNumber->setText(strInput);
    }
    else
    {
        strInput ="";
        this->value =0.0;
        txtNumber->setText(strInput);
    }

}


void dlgLevelInputBox::on_btnA_clicked()
{
    this->unit="A";
    this->accept();
}

void dlgLevelInputBox::on_btn7_clicked()
{
    numberInput(7);
}

void dlgLevelInputBox::on_btn8_clicked()
{
    numberInput(8);
}

void dlgLevelInputBox::on_btn9_clicked()
{
    numberInput(9);
}

void dlgLevelInputBox::on_btn4_clicked()
{
    numberInput(4);
}

void dlgLevelInputBox::on_btn5_clicked()
{
    numberInput(5);
}

void dlgLevelInputBox::on_btn6_clicked()
{
    numberInput(6);
}

void dlgLevelInputBox::on_btn1_clicked()
{
    numberInput(1);
}

void dlgLevelInputBox::on_btn2_clicked()
{
    numberInput(2);
}

void dlgLevelInputBox::on_btn3_clicked()
{
    numberInput(3);
}

void dlgLevelInputBox::on_btn0_clicked()
{
    if(!firstInput)
    {
        on_btnClear_clicked();
        firstInput = true;
    }

    if(stopInput)
        return;

    if(strInput.contains("."))
    {
        strInput +="0";

    }
    else if(strInput.length() ==0)
    {
        strInput +="0";
    }
    else if(this->value !=0)
    {
         strInput +="0";
    }

    txtNumber->setText(strInput);

    bool ok;
    double dbl = strInput.toDouble(&ok);
    if(ok)
    {
        this->value = dbl;
    }



}

void dlgLevelInputBox::on_btnDota_clicked()
{
    if(!firstInput)
    {
        on_btnClear_clicked();
        firstInput = true;
    }

    if(stopInput)
        return;

    if(strInput.contains("."))
        return;

    if(strInput.isEmpty())
        strInput="0.";
    else
        strInput+=".";

    txtNumber->setText(strInput);


}

void dlgLevelInputBox::on_btnDel_clicked()
{
    stopInput= false;
    firstInput= true;

    if(!suffix.isEmpty())
    {
        suffix ="";
        txtNumber->setText(strInput);
        return;
    }
    if(!strInput.isEmpty())
    {

        strInput = strInput.remove(strInput.length()-1,1);
        bool ok;
        double dbl = strInput.toDouble(&ok);

        if(ok)
        {
            this->value = dbl;

        }
        else
        {

            this->value =0.0;
            strInput ="0";
            firstInput = false;
        }
        txtNumber->setText(strInput);
    }
}
