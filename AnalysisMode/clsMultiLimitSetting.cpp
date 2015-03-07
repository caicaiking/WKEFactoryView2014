#include "clsMultiLimitSetting.h"

clsMultiLimitSetting::clsMultiLimitSetting(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(this->lblFrequency,SIGNAL(Clicked()),this,SLOT(lblFrequencClick()));
    connect(this->lblItem1,SIGNAL(Clicked()),this,SLOT(lblItem1Click()));
    connect(this->lblItem2,SIGNAL(Clicked()),this,SLOT(lblItem2Click()));
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
void clsMultiLimitSetting::updateLabels()
{
    this->grpTraceA->setTitle(tr("%1 上下限").arg(this->item1));
    this->grpTraceB->setTitle(tr("%2上下限").arg(this->item2));
    lblFrequency->setText(this->limits.getFreqRange());
    lblItem1->setText(this->limits.getLimit1().showLimits(item1));
    lblItem2->setText(this->limits.getLimit2().showLimits(item2));
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

}

void clsMultiLimitSetting::lblItem2Click()
{

}

void clsMultiLimitSetting::lblFrequencClick()
{

}
