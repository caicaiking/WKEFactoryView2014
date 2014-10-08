#include "cls4300MeterMode.h"
#include "clsRuningSettings.h"
#include <QDebug>
#include "frmSetLimit.h"
#include "clsMeterLimit.h"
#include "UserfulFunctions.h"
#include "dlgLevelInputBox.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "dlg4300FuctionMajor.h"
#include "dlg4300FunctionMinor.h"
cls4300MeterMode::cls4300MeterMode(QWidget *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);

    //链接上下限按钮的信号。
    connect(this->btnTest1Limit1,SIGNAL(Clicked()),this,SLOT(setTest1Limit1()));
    connect(this->btnTest2Limit1,SIGNAL(Clicked()),this,SLOT(setTest2Limit1()));
    connect(this->btnTest1Limit2,SIGNAL(Clicked()),this,SLOT(setTest1Limit2()));
    connect(this->btnTest2Limit2,SIGNAL(Clicked()),this,SLOT(setTest2Limit2()));


    this->items<<"Z"<<"A"<<"L"<<"Q";
    clsMeterLimit m1,m2,m3,m4;
    meterLimit<<m1<<m2<<m3<<m4;
    frequency<<1000<<10000;
    level<<0.5<<1;
    equcct<<tr("串联")<<tr("并联");
    range =tr("自动");
    speed =tr("最快");
    updateButtons();

}

void cls4300MeterMode::updateButtons()
{
    btnTest1Item1->setText(items.at(0));
    if(btnTest1Item1->text().toUpper()==tr("Rdc").toUpper())
    {
        grpTest1Item2->setEnabled(false);
        grpTest1Item2->setVisible(false);
    }
    else
    {
        grpTest1Item2->setEnabled(true);
        grpTest1Item2->setVisible(true);
    }


    btnTest1Item2->setText(items.at(1));
    btnTest2Item1->setText(items.at(2));
    if(btnTest2Item1->text().toUpper()==tr("Rdc").toUpper())
    {
        grpTest2Item2->setEnabled(false);
        grpTest2Item2->setVisible(false);
    }
    else
    {
        grpTest2Item2->setEnabled(true);
        grpTest2Item2->setVisible(true);
    }

    btnTest2Item2->setText(items.at(3));

    clsMeterLimit tmp;
    tmp = meterLimit.at(0);
    btnTest1Limit1->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest1Item1->text())));

    tmp = meterLimit.at(1);
    btnTest1Limit2->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest1Item2->text())));

    tmp = meterLimit.at(2);
    btnTest2Limit1->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest2Item1->text())));

    tmp = meterLimit.at(3);
    btnTest2Limit2->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest2Item2->text())));

    doubleType dt;
    dt.setData(frequency.at(0),"");
    btnTest1Frequency->setText(dt.formateToString()+"Hz");

    dt.setData(frequency.at(1),"");
    btnTest2Frequency->setText(dt.formateToString()+"Hz");

    dt.setData(level.at(0),"");
    btnTest1Level->setText(dt.formateToString()+"V");
    dt.setData(level.at(1),"");
    btnTest2Level->setText(dt.formateToString()+"V");

    btnTest1Range->setText(this->range);
    btnTest1Speed->setText(this->speed);

    btnTest1Equcct->setText(equcct.at(0));
    btnTest2Equcct->setText(equcct.at(1));

}

FunctionType cls4300MeterMode::getType()
{
    return MeterFunction;
}

void cls4300MeterMode::setCondition(const QString &value)
{

}

QString cls4300MeterMode::getTestItem()
{

}

void cls4300MeterMode::trig()
{
    qDebug()<< clsRS::getInst().sendCommand(":MEAS:TRIG",true);
}

void cls4300MeterMode::start()
{

}

QString cls4300MeterMode::getTestCondition()
{

}

QString cls4300MeterMode::getBrief()
{

}

bool cls4300MeterMode::getPassFail()
{

}

void cls4300MeterMode::setTest1Limit1()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(0));
    dlg->setItem(btnTest1Item1->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(0,dlg->getMeterLimit());
        this->btnTest1Limit1->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest1Item1->text())));
    }
}

void cls4300MeterMode::setTest2Limit1()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(2));
    dlg->setItem(btnTest2Item1->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(2,dlg->getMeterLimit());
        this->btnTest2Limit1->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest2Item1->text())));
    }
}

void cls4300MeterMode::setTest1Limit2()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(1));
    dlg->setItem(btnTest1Item2->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(1,dlg->getMeterLimit());
        this->btnTest1Limit2->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest1Item2->text())));
    }
}

void cls4300MeterMode::setTest2Limit2()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(3));
    dlg->setItem(btnTest2Item2->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(3,dlg->getMeterLimit());
        this->btnTest2Limit2->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest2Item2->text())));
    }
}


double cls4300MeterMode::getMaxFrequency1(QString value)
{
    if(value.contains("41100"))
    {
        return 1000000;
    }
    else if(value.contains("4150"))
    {
        return 500000;
    }
    else if(value.contains("4120"))
    {
        return 200000;
    }
    else if(value.contains("4110"))
    {
        return 100000;
    }
    else if(value.contains("43100"))
    {
        return 1000000;
    }
    else if(value.contains("4350"))
    {
        return 500000;
    }
    else if(value.contains("4320"))
    {
        return 200000;
    }
    else if(value.contains("4310"))
    {
        return 100000;
    }

    return 1000000;
}



void cls4300MeterMode::on_btnTest1Frequency_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入频率"));

    if(dlg->exec())
    {
        double value = dlg->getNumber();
        double maxFreuqncy = getMaxFrequency1(clsRS::getInst().instrumentModel);
        value =(value>maxFreuqncy? maxFreuqncy:value);
        value = (value<20? 20:value);
        frequency.replace(0,value);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Frequency_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入频率"));

    if(dlg->exec())
    {
        double value = dlg->getNumber();
        double maxFreuqncy = getMaxFrequency1(clsRS::getInst().instrumentModel);
        value =(value>maxFreuqncy? maxFreuqncy:value);
        value = (value<20? 20:value);
        frequency.replace(1,value);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Level_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox(this);
    dlg->setAVisible(false);
    dlg->setWindowTitle("输入电平");

    if(dlg->exec())
    {
        double value=dlg->getValue();
        QString suffix=dlg->getSuffix();
        doubleType dt;
        dt.setData(value,suffix);

        double number = dt.Data();
        number=(number>2?2:number);
        number=(number<0.01?0.01:number);
        level.replace(0,number);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Level_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox(this);
    dlg->setAVisible(false);
    dlg->setWindowTitle("输入电平");

    if(dlg->exec())
    {
        double value=dlg->getValue();
        QString suffix=dlg->getSuffix();
        doubleType dt;
        dt.setData(value,suffix);

        double number = dt.Data();
        number=(number>2?2:number);
        number=(number<0.01?0.01:number);
        level.replace(1,number);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Range_clicked()
{
    wk6500Range *dlg = new wk6500Range(this);
    dlg->setWindowTitle(tr("设置测试速度"));
    if(dlg->exec())
    {
        range = dlg->getRange();
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Speed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置测试速度"));

    if(dlg->exec())
    {
        this->speed= dlg->getSpeed();
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Equcct_clicked()
{
    if(equcct.at(0)==tr("串联"))
    {
        equcct.replace(0,tr("并联"));
    }
    else
    {
        equcct.replace(0,tr("串联"));
    }
    updateButtons();
}

void cls4300MeterMode::on_btnTest2Equcct_clicked()
{
    if(equcct.at(1)==tr("串联"))
    {
        equcct.replace(1,tr("并联"));
    }
    else
    {
        equcct.replace(1,tr("串联"));
    }
    updateButtons();
}

void cls4300MeterMode::on_btnTest1Item1_clicked()
{
    dlg4300FuctionMajor *dlg = new dlg4300FuctionMajor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(0, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Item2_clicked()
{
    dlg4300FunctionMinor *dlg = new dlg4300FunctionMinor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(1, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Item1_clicked()
{
    dlg4300FuctionMajor *dlg = new dlg4300FuctionMajor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(2, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Item2_clicked()
{
    dlg4300FunctionMinor *dlg = new dlg4300FunctionMinor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(3, dlg->getItem());
        updateButtons();
    }
}
