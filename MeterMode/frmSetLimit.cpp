#include "frmSetLimit.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "NumberInput.h"
frmSetLimit::frmSetLimit(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

clsMeterLimit frmSetLimit::getMeterLimit()
{
    return m1;
}

void frmSetLimit::setLimits(clsMeterLimit value)
{
    this->m1=value;
    updateButtons();
}

void frmSetLimit::setItem(QString value)
{
    this->item=value;
    updateButtons();
}

void frmSetLimit::on_rbAbs_toggled(bool checked)
{
    rbPer->setChecked(!checked);
    wdgAbs->setEnabled(checked);
    wdgPer->setEnabled(!checked);
    if(checked)
        m1.setLimitType("Norm");
}

void frmSetLimit::on_rbPer_toggled(bool checked)
{
    rbAbs->setChecked(!checked);
    wdgAbs->setEnabled(!checked);
    wdgPer->setEnabled(checked);
    if(checked)
        m1.setLimitType("Per");

}

void frmSetLimit::updateButtons()
{
    if(m1.getLimitType()=="Norm")
        this->rbAbs->setChecked(true);
    else
        this->rbPer->setChecked(true);

    doubleType dt;
    QString dQ ="DQ";
    QString unit =UserfulFunctions::getSuffix(item);
    if(!dQ.contains(item))
    {
        dt.setData(m1.getAbsHi(),"");
        this->btnAbsHi->setText(dt.formateToString()+unit);

        dt.setData(m1.getAbsLo(),"");
        this->btnAbsLow->setText(dt.formateToString()+unit);

        dt.setData(m1.getNorminal(),"");
        this->btnPerNorminal->setText(dt.formateToString()+unit);
    }
    else
    {
        dt.setData(m1.getAbsHi(),"");
        this->btnAbsHi->setText(dt.formateWithUnit("")+unit);

        dt.setData(m1.getAbsLo(),"");
        this->btnAbsLow->setText(dt.formateWithUnit("")+unit);

        dt.setData(m1.getNorminal(),"");
        this->btnPerNorminal->setText(dt.formateWithUnit("")+unit);
    }

    dt.setData(m1.getPerHi(),"");
    btnPerHi->setText(dt.formateWithUnit("",5)+"%");

    dt.setData(m1.getPerLo(),"");
    btnPerLo->setText(dt.formateWithUnit("",6)+"%");

}


void frmSetLimit::on_btnW_clicked()
{
    m1.setPerHi(0.05);
    m1.setPerLo(-0.05);
    updateButtons();
}

void frmSetLimit::on_btnN_clicked()
{
    m1.setPerHi(30);
    m1.setPerLo(-30);
    updateButtons();
}

void frmSetLimit::on_btnM_clicked()
{
    m1.setPerHi(20);
    m1.setPerLo(-20);
    updateButtons();
}

void frmSetLimit::on_btnK_clicked()
{
    m1.setPerHi(10);
    m1.setPerLo(-10);
    updateButtons();
}

void frmSetLimit::on_btnJ_clicked()
{
    m1.setPerHi(5);
    m1.setPerLo(-5);
    updateButtons();
}

void frmSetLimit::on_btnG_clicked()
{
    m1.setPerHi(2);
    m1.setPerLo(-2);
    updateButtons();
}

void frmSetLimit::on_btnF_clicked()
{
    m1.setPerHi(1);
    m1.setPerLo(-1);
    updateButtons();
}

void frmSetLimit::on_btnD_clicked()
{
    m1.setPerHi(0.5);
    m1.setPerLo(-0.5);
    updateButtons();
}

void frmSetLimit::on_btnC_clicked()
{
    m1.setPerHi(0.25);
    m1.setPerLo(-0.25);
    updateButtons();
}

void frmSetLimit::on_btnB_clicked()
{
    m1.setPerHi(0.1);
    m1.setPerLo(-0.1);
    updateButtons();
}

void frmSetLimit::on_btnAbsHi_clicked()
{
    NumberInput *dlg = new NumberInput(/*m1.getAbsHi(),""*/);
    dlg->setWindowTitle(tr("输入上限值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        m1.setAbsHi( dlg->getNumber());
        updateButtons();
    }

}

void frmSetLimit::on_btnAbsLow_clicked()
{
    NumberInput *dlg = new NumberInput(/*m1.getAbsLo(),""*/);
    dlg->setWindowTitle(tr("输入下限值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        m1.setAbsLo( dlg->getNumber());
        updateButtons();
    }
}

void frmSetLimit::on_btnPerNorminal_clicked()
{
    NumberInput *dlg = new NumberInput(/*m1.getNorminal(),""*/);
    dlg->setWindowTitle(tr("输入中心值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        m1.setNorminal(dlg->getNumber());
        updateButtons();
    }
}

void frmSetLimit::on_btnPerHi_clicked()
{
    NumberInput *dlg = new NumberInput(/*m1.getPerHi(),""*/);
    dlg->setWindowTitle(tr("输入上限值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        m1.setPerHi( dlg->getNumber());
        updateButtons();
    }
}

void frmSetLimit::on_btnPerLo_clicked()
{
    NumberInput *dlg = new NumberInput(/*m1.getPerLo(),""*/);
    dlg->setWindowTitle(tr("输入下限值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        m1.setPerLo( dlg->getNumber());
        updateButtons();
    }
}

void frmSetLimit::on_btnOk_clicked()
{
    this->accept();
}

void frmSetLimit::on_btnCancel_clicked()
{
    this->reject();
}
