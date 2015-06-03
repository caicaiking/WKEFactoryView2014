#include "clsMultiLimitSetting.h"
#include "UserfulFunctions.h"
#include "frmSetLimit.h"
clsMultiLimitSetting::clsMultiLimitSetting(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(this->lblFrequency,SIGNAL(Clicked()),this,SLOT(lblFrequencClick()));
    connect(this->lblItem1,SIGNAL(Clicked()),this,SLOT(lblItem1Click()));
    connect(this->lblItem2,SIGNAL(Clicked()),this,SLOT(lblItem2Click()));
    updateLabels();
}

void clsMultiLimitSetting::setConditon(const QString &item1, const QString &item2,
                                       const clsMultiLimits &limits)
{
    this->item1 = item1;
    this->item2 = item2;
    this->limits = limits;
    this->limits.setItems(item1,item2);
    updateLabels();
}

clsMultiLimits clsMultiLimitSetting::getLimts()
{
    return this->limits;
}
void clsMultiLimitSetting::updateLabels()
{
    this->grpTraceA->setTitle(tr("%1 上下限").arg(this->item1));
    this->grpTraceB->setTitle(tr("%2上下限").arg(this->item2));
    lblFrequency->setText(this->limits.getFreqRange());
    lblItem1->setText(this->limits.getLimit1().showLimits(UserfulFunctions::getSuffix(item1)));
    lblItem2->setText(this->limits.getLimit2().showLimits(UserfulFunctions::getSuffix(item2)));
    grpTraceA->setChecked(this->limits.getBlLimit1());
    grpTraceB->setChecked(this->limits.getBlLimit2());
}

void clsMultiLimitSetting::on_btnOk_clicked()
{
    this->accept();
}

void clsMultiLimitSetting::on_btnCancel_clicked()
{
    this->reject();
}

void clsMultiLimitSetting::lblItem1Click()
{
    frmSetLimit dlg;
    dlg.setWindowTitle(tr("设定%1上下限").arg(this->item1));
    dlg.setItem(this->item1);
    dlg.setLimits(this->limits.getLimit1());

    if(dlg.exec())
    {
        this->limits.setLimit1(dlg.getMeterLimit());
        updateLabels();
    }
}

void clsMultiLimitSetting::lblItem2Click()
{
    frmSetLimit dlg;
    dlg.setWindowTitle(tr("设定%1上下限").arg(this->item2));
    dlg.setItem(this->item2);
    dlg.setLimits(this->limits.getLimit2());

    if(dlg.exec())
    {
        this->limits.setLimit2(dlg.getMeterLimit());
        updateLabels();
    }
}

void clsMultiLimitSetting::lblFrequencClick()
{
    frmSetLimit dlg;
    dlg.setWindowTitle(tr("设定%1上下限").arg(tr("频率")));
    dlg.setItem("Hz");
    dlg.setLimits(this->limits.getFreq());

    if(dlg.exec())
    {
        this->limits.setFreq(dlg.getMeterLimit());
        updateLabels();
    }
}


void clsMultiLimitSetting::on_grpTraceA_toggled(bool arg1)
{
    this->limits.setBlLimit1(arg1);
}

void clsMultiLimitSetting::on_grpTraceB_toggled(bool arg1)
{
    this->limits.setBlLimit2(arg1);
}
