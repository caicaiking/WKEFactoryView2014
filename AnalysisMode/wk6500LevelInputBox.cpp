#include "wk6500LevelInputBox.h"

wk6500LevelInputBox::wk6500LevelInputBox(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    stopInput=false;
}

wk6500LevelInputBox::wk6500LevelInputBox(const double value, const QString suffix, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->value =value;
    this->suffix = suffix;
    firstInput = false;
    stopInput = false;
    strInput =QString::number(value);

    txtNumber->setText(QString::number(value)+suffix);
}

void wk6500LevelInputBox::setValueAndSuffix(const double value, const QString suffix)
{
    this->value =value;
    this->suffix = suffix;
    firstInput = false;
    stopInput = false;
    strInput =QString::number(value);
    txtNumber->setText(QString::number(value)+suffix);
}

void wk6500LevelInputBox::setAmpEnable(bool status)
{
    btnA->setEnabled(status);
}

void wk6500LevelInputBox::on_btnClear_clicked()
{

    txtNumber->clear();
    strInput ="";
    this->value=0;
    this->suffix="";
    firstInput = false;
    stopInput = false;
}

void wk6500LevelInputBox::on_btnV_clicked()
{
    this->unit="V";
    this->accept();

}

void wk6500LevelInputBox::on_btnG_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("G");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnM_clicked()
{
    if (!strInput.isEmpty() && (!stopInput))
    {

        this->suffix=("M");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnk_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("k");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnm_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("m");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnu_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("u");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnn_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("n");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnp_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("p");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnf_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("f");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void wk6500LevelInputBox::on_btnCancel_clicked()
{
    this->reject();
}

void wk6500LevelInputBox::numberInput(int i)
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

void wk6500LevelInputBox::signInput(QString)
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


void wk6500LevelInputBox::on_btnA_clicked()
{
    this->unit="A";
    this->accept();
}

void wk6500LevelInputBox::on_btn7_clicked()
{
    numberInput(7);
}

void wk6500LevelInputBox::on_btn8_clicked()
{
    numberInput(8);
}

void wk6500LevelInputBox::on_btn9_clicked()
{
    numberInput(9);
}

void wk6500LevelInputBox::on_btn4_clicked()
{
    numberInput(4);
}

void wk6500LevelInputBox::on_btn5_clicked()
{
    numberInput(5);
}

void wk6500LevelInputBox::on_btn6_clicked()
{
    numberInput(6);
}

void wk6500LevelInputBox::on_btn1_clicked()
{
    numberInput(1);
}

void wk6500LevelInputBox::on_btn2_clicked()
{
    numberInput(2);
}

void wk6500LevelInputBox::on_btn3_clicked()
{
    numberInput(3);
}

void wk6500LevelInputBox::on_btn0_clicked()
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

void wk6500LevelInputBox::on_btnDota_clicked()
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

void wk6500LevelInputBox::on_btnDel_clicked()
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
