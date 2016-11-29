#include "clsStandardValueInput.h"
#include "NumberInput.h"
#include "Globle.h"
#include "UserfulFunctions.h"
#include "complexType.h"

clsStandardValueInput::clsStandardValueInput(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    z=OR;
    a=OR;
    dblItem1=OR;
    dblItem2=OR;
    item1 ="Z";
    item2 ="θ";
    equcct = tr("串联");
}

void clsStandardValueInput::setFrequency(double value)
{
    this->frequency = value;
    doubleType dt;
    dt.setData(frequency,"");
    lblFrequency->setText(tr("频率：")+dt.formateToString()+"Hz");
}

void clsStandardValueInput::on_cmdItems_currentIndexChanged(int /*index*/)
{
    QString items = cmdItems->currentText();
    QStringList list = items.split(" ");

    if(list.length()<2)
        return;
    lblItem1->setText(list.at(0));
    lblItem2->setText(list.at(1));
    item1 = list.at(0);
    item2 = list.at(1);

    updateText();
}

void clsStandardValueInput::on_btnItem1_clicked()
{
    NumberInput *dlg = new NumberInput();
    dlg->setWindowTitle(tr("输入标准%1值").arg(lblItem1->text()));

    if(dlg->exec()== QDialog::Accepted)
    {
        dblItem1= dlg->getNumber();
        updateText();
    }
}
double clsStandardValueInput::getA() const
{
    return a;
}
double clsStandardValueInput::getZ() const
{
    return z;
}


void clsStandardValueInput::on_btnItem2_clicked()
{
    NumberInput *dlg = new NumberInput();
    dlg->setWindowTitle(tr("输入标准%1值").arg(lblItem2->text()));

    if(dlg->exec()== QDialog::Accepted)
    {
        dblItem2= dlg->getNumber();
        updateText();
    }
}

void clsStandardValueInput::updateText()
{
    doubleType dt;
    if(dblItem1 != OR)
    {
        dt.setData(dblItem1,"");
        btnItem1->setText(dt.formateToString()+UserfulFunctions::getSuffix(item1));
    }
    else
        btnItem1->setText(tr("点击输入"));

    dt.setData(dblItem2,"");
    if(dblItem2 != OR)
    {
        if(QString("QDAθ").contains(item2))
            btnItem2->setText(dt.formateWithUnit("")+UserfulFunctions::getSuffix(item2));
        else
            btnItem2->setText(dt.formateToString()+UserfulFunctions::getSuffix(item2));
    }
    else
        btnItem2->setText(tr("点击输入"));

}

void clsStandardValueInput::on_btnOk_clicked()
{

    if((dblItem1 == OR) || (dblItem2 == OR))
        return;

    int index = this->cmdItems->currentIndex();
    double factor =(equcct ==tr("串联")? 1.0: -1.0);

    switch (index) {
    case 0: //ZA
        z= dblItem1;
        a = factor*dblItem2;
        break;
    case 1: //RX
        if(factor ==1)
        {
            double Rs = dblItem1;
            double Xs = dblItem2;
            convertRsXsToZA(Rs,Xs);
        }
        else
        {
            double Rp = dblItem1;
            double Xp = dblItem2;

            convertRpXpToZA(Rp,Xp);
        }
        break;
    case 2: //LQ
        if(factor ==1) //series
        {
            double Xs = 2.0*PI*frequency*dblItem1;
            double Q = dblItem2;
            double Rs;

            if(Q>=0.0)
                Rs = std::abs(Xs/dblItem2);
            else
                Rs = -1.0*std::abs(Xs/dblItem2);

            convertRsXsToZA(Rs,Xs);
        }
        else //Parallel
        {
            double Q = dblItem2;
            double Lp = dblItem1;
            double Xp = 2.0*PI*frequency*Lp;
            double Rp;

            if(Q>=0.0)
                Rp = std::abs(Xp*Q);
            else
                Rp = -1.0*std::abs(Xp*Q);
            convertRpXpToZA(Rp,Xp);
        }
        break;
    case 3: //CD
        if(factor ==1 ) //Series
        {
            double Cs = dblItem1;
            double D = dblItem2;
            double Xs = (Cs>=0.0?-1.0:1.0) *1.0/(2.0*PI*frequency*Cs);
            double Rs;
            if(D>=0.0)
                Rs= std::abs(Xs*D);
            else
                Rs = -1.0*std::abs(Xs*D);
            this->convertRsXsToZA(Rs,Xs);
        }
        else //Parallel
        {//Cp Dp
            double Cp = dblItem1;
            double D = dblItem2;
            double Cs = Cp*(1.0+D*D);
            double Xs = (Cp>=0.0?-1.0:1.0) *1.0/(2.0*PI*frequency*Cs);
            double Rs;
            if(D>=0.0)
                Rs=std::abs(Xs*D);
            else
                Rs=-1.0*std::abs(Xs*D);
            this->convertRsXsToZA(Rs,Xs);
        }
        break;
    case 4: //LR

        if(factor==1)
        {
            double Ls = dblItem1;
            double Rs = dblItem2;
            double Xs = 2.0*PI*frequency*Ls;
            this->convertRsXsToZA(Rs,Xs);
        }
        else
        {
            double Lp = dblItem1;
            double Rp = dblItem2;
            double Xp = 2.0*PI*frequency*Lp;
            this->convertRpXpToZA(Rp,Xp);
        }

        break;
    case 5: //CR
        if(factor == 1)
        {
            double Cs = dblItem1;
            double Rs = dblItem2;

            double Xs =-1.0/(2.0*PI*frequency*Cs);
            this->convertRsXsToZA(Rs,Xs);
        }
        else
        {
            double Cp = dblItem1;
            double Rp = dblItem2;

            double Xp = -1.0/(2.0*PI*frequency*Cp);
            this->convertRpXpToZA(Rp,Xp);
        }
        break;
    default:
        break;
    }

    this->accept();
}


void clsStandardValueInput::convertRpXpToZA(double Rp, double Xp)
{
    double Qp = Rp/Xp;
    double Rs = Rp/(1.0+Qp*Qp);
    double Xs = Xp/(1.0+1.0/(Qp*Qp));

    convertRsXsToZA(Rs,Xs);
}

void clsStandardValueInput::convertRsXsToZA(double Rs, double Xs)
{
    complexType cp;
    cp.setRX(Rs,Xs);

    z = cp.getZ();
    a = cp.getA();
    //qDebug() << "z:" << z <<"\ta:"<<a;
}

void clsStandardValueInput::on_btnEqucct_clicked()
{
    equcct = (equcct==tr("串联")?tr("并联"):tr("串联"));
    btnEqucct->setText(equcct);
}

void clsStandardValueInput::on_btnCancel_clicked()
{
    this->reject();
}
