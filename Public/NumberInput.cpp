 #include "NumberInput.h"

NumberInput::NumberInput(QWidget *parent) :
    QDialog(parent)
{

    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    stopInput=false;
}

NumberInput::NumberInput(const double value, const QString suffix, QWidget *parent):
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

void NumberInput::setValueAndSuffix(const double value, const QString suffix)
{
    doubleType dt(value,suffix);
    dt.getDataAndUnit(this->value,this->suffix);

    //qDebug()<< dt.formateToString();

    firstInput = false;
    stopInput = false;
    strInput =dt.formateWithUnit(this->suffix);
    txtNumber->setText(dt.formateToString());
}

void NumberInput::on_btnClear_clicked()
{

    txtNumber->clear();
    strInput ="";
    this->value=0;
    this->suffix="";
    firstInput = false;
    stopInput = false;
}

void NumberInput::on_btnG_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("G");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnM_clicked()
{
    if (!strInput.isEmpty() && (!stopInput))
    {

        this->suffix=("M");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnk_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("k");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnm_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("m");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnu_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("u");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnn_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("n");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnp_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("p");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnf_clicked()
{
    if (!strInput.isEmpty()&& (!stopInput))
    {

        this->suffix=("f");
        stopInput= true;
    }

    this->txtNumber->setText(strInput+suffix);
}

void NumberInput::on_btnCancel_clicked()
{
    this->reject();
}

void NumberInput::numberInput(int i)
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

void NumberInput::signInput()
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


void NumberInput::on_btnSign_clicked()
{
    signInput();
}

void NumberInput::on_btn7_clicked()
{
    numberInput(7);
}

void NumberInput::on_btn8_clicked()
{
    numberInput(8);
}

void NumberInput::on_btn9_clicked()
{
    numberInput(9);
}

void NumberInput::on_btn4_clicked()
{
    numberInput(4);
}

void NumberInput::on_btn5_clicked()
{
    numberInput(5);
}

void NumberInput::on_btn6_clicked()
{
    numberInput(6);
}

void NumberInput::on_btn1_clicked()
{
    numberInput(1);
}

void NumberInput::on_btn2_clicked()
{
    numberInput(2);
}

void NumberInput::on_btn3_clicked()
{
    numberInput(3);
}

void NumberInput::on_btn0_clicked()
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

void NumberInput::on_btnDota_clicked()
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

void NumberInput::on_btnDel_clicked()
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

void NumberInput::on_btnOk_clicked()
{
    this->accept();
}
