#include "clsMultModeMeterUi.h"
#include "cls6440MultiMeterMode.h"
#include <QDebug>

#include "clsMeterLimit.h"
#include "clsMultiChannelMeterFactory.h"
#include "clsRuningSettings.h"
clsMultiModeMeterUi::clsMultiModeMeterUi(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    meter = clsMultiChannelMeterFactory::getMeter(clsRS::getInst().meterSeries);
    //链接上下限按钮的信号。
    connect(this->btnTest1Limit1,SIGNAL(Clicked()),this,SLOT(setTest1Limit1()));
    connect(this->btnTest2Limit1,SIGNAL(Clicked()),this,SLOT(setTest2Limit1()));
    connect(this->btnTest1Limit2,SIGNAL(Clicked()),this,SLOT(setTest1Limit2()));
    connect(this->btnTest2Limit2,SIGNAL(Clicked()),this,SLOT(setTest2Limit2()));


}

void clsMultiModeMeterUi::setConditon(QString value)
{
    this->meter->setCondition(value);
    updataButtons();
}

QString clsMultiModeMeterUi::getConditon()
{
    return meter->getConditon();
}

void clsMultiModeMeterUi::setTest1Limit1()
{
    meter->setTest1Item1Limit();
    updataButtons();
}

void clsMultiModeMeterUi::setTest2Limit1()
{
    meter->setTest2Item1Limit();
    updataButtons();
}

void clsMultiModeMeterUi::setTest1Limit2()
{
    meter->setTest1Item2Limit();
    updataButtons();
}

void clsMultiModeMeterUi::setTest2Limit2()
{
    meter->setTest2Item2Limit();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Frequency_clicked()
{
    meter->setTest1Freq();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Frequency_clicked()
{
    meter->setTest2Freq();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Level_clicked()
{
    meter->setTest1Level();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Level_clicked()
{
    meter->setTest2Level();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Range_clicked()
{
    meter->setRange();
    updataButtons();

}

void clsMultiModeMeterUi::on_btnTest1Speed_clicked()
{
    meter->setSpeed();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Equcct_clicked()
{
    meter->setTest1Equcct();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Equcct_clicked()
{
    meter->setTest2Equcct();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Item1_clicked()
{
    meter->setTest1Item1();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Item2_clicked()
{
    meter->setTest1Item2();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Item1_clicked()
{
    meter->setTest2Item1();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Item2_clicked()
{
    meter->setTest2Item2();
    updataButtons();
}




void clsMultiModeMeterUi::on_btnOk_clicked()
{
    this->accept();
}

void clsMultiModeMeterUi::updataButtons()
{
    if(meter==0)
        return;

    btnTest1Frequency->setText(meter->getTest1Freq());
    btnTest1Level->setText(meter->getTest1Level());
    btnTest1Equcct->setText(meter->getTest1Equcct());

    btnTest1Range->setText(meter->getRange());
    btnTest1Speed->setText(meter->getSpeed());

    btnTest1Item1->setText(meter->getTest1Item1());
    btnTest1Item2->setText(meter->getTest1Item2());

    btnTest1Item1Unit->setText(meter->getTest1Item1Unit());
    btnTest1Item2Unit->setText(meter->getTest1Item2Unit());

    btnTest1Limit1->setText(meter->getTest1Item1Limit());
    btnTest1Limit2->setText(meter->getTest1Item2Limit());

    grpTest1Item2->setChecked(meter->getTest1Item2Enable());

    grpTest2->setChecked(meter->getTest2On());

    btnTest2Frequency->setText(meter->getTest2Freq());
    btnTest2Level->setText(meter->getTest2Level());
    btnTest2Equcct->setText(meter->getTest2Equcct());

    btnTest2Item1->setText(meter->getTest2Item1());
    btnTest2Item1Unit->setText(meter->getTest2Item1Unit());
    btnTest2Limit1->setText(meter->getTest2Item1Limit());

    btnTest2Item2->setText(meter->getTest2Item2());
    btnTest2Item2Unit->setText(meter->getTest2Item2Unit());
    btnTest2Limit2->setText(meter->getTest2Item2Limit());

    grpTest2Item2->setChecked(meter->getTest2Item2Enable());

}

void clsMultiModeMeterUi::on_grpTest1Item2_toggled(bool arg1)
{
    meter->setTest1Item2Enable(arg1);
    updataButtons();
}

void clsMultiModeMeterUi::on_grpTest2_toggled(bool arg1)
{
    meter->setTest2On(arg1);
    updataButtons();
}

void clsMultiModeMeterUi::on_grpTest2Item2_toggled(bool arg1)

{
    meter->setTest2Item2Enable(arg1);
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Item1Unit_clicked()
{
    this->meter->setTest1Item1Unit();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest1Item2Unit_clicked()
{
    meter->setTest1Item2Unit();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Item1Unit_clicked()
{
    this->meter->setTest2Item1Unit();
    updataButtons();
}

void clsMultiModeMeterUi::on_btnTest2Item2Unit_clicked()
{
    this->meter->setTest2Item2Unit();
    updataButtons();
}
