#include "cls6440MeterMode.h"
#include <QDebug>
#include "frmSetLimit.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "clsMeterUnit.h"
cls6440MeterMode::cls6440MeterMode(WKEMeterMode *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);

    connect(lblMaijor,SIGNAL(Clicked()),this,SLOT(limitMajorClick()));
    connect(lblMinor,SIGNAL(Clicked()),this,SLOT(limitMinorClick()));
    connect(lblRdc,SIGNAL(Clicked()),this,SLOT(limitRdcClick()));

    strDescription = tr("空白");
    item1="Z";
    item2="θ";
    level =1;
    levelType="V";
    frequency=10000;
    biasSource = tr("内置");
    blBiasStatus= false;
    range = tr("自动");
    speed = tr("最快");
    equcct = tr("串联");
    levelRdc = 1;
    majorUnit="";
    minorUnit="";
    rdcUnit="";

    updateButtons();
}

FunctionType cls6440MeterMode::getFunctionType()
{

}

void cls6440MeterMode::setFunctionType(FunctionType value)
{

}

bool cls6440MeterMode::detectDut()
{

}

void cls6440MeterMode::stopDetect()
{

}

void cls6440MeterMode::setCondition(QString value)
{

}

QString cls6440MeterMode::getConditon()
{

}

void cls6440MeterMode::setTestMode(TestMode value)
{

}

void cls6440MeterMode::updateGPIB()
{

}

bool cls6440MeterMode::getTotalStatus()
{

}

int cls6440MeterMode::getCountTestItems()
{

}

QString cls6440MeterMode::getItem(int i)
{

}

clsMeterLimit cls6440MeterMode::getLimit(int i)
{

}

void cls6440MeterMode::calibration()
{

}

QString cls6440MeterMode::getBrief()
{

}

QString cls6440MeterMode::getMeterSeries()
{

}

QString cls6440MeterMode::getDescription()
{

}

void cls6440MeterMode::limitMajorClick()
{

}

void cls6440MeterMode::limitMinorClick()
{

}

void cls6440MeterMode::limitRdcClick()
{

}

void cls6440MeterMode::updateButtons()
{
    btnItem1->setText(item1);
    QString zy ="ZY";
    if(zy.contains(item1))
    {
        item2 = "θ";
        btnItem2->setText(item2);
    }

    doubleType dt;
    dt.setData(level,"");
    btnLevel->setText(dt.formateToString()+levelType);

    dt.setData(frequency);
    btnFrequency->setText(dt.formateToString()+"Hz");

    btnBiasSource->setText(biasSource);
    btnBiasStatus->setText(blBiasStatus?tr("开"):tr("关"));

    btnRange->setText(range);
    btnSpeed->setText(speed);
    btnEqucct->setText(equcct);

    lblMaijor->setText(lmMajor.showLimits(UserfulFunctions::getSuffix(item1)));
    lblMinor->setText(lmMinor.showLimits(UserfulFunctions::getSuffix(item2)));
    lblRdc->setText(lmRdc.showLimits(UserfulFunctions::getSuffix(QString("RDC"))));

    dt.setData(levelRdc);
    btnRdcLevel->setText(dt.formateToString(6)+"V");

    btnMajorUnit->setText(majorUnit+UserfulFunctions::getSuffix(item1));
    btnMinorUnit->setText(minorUnit+UserfulFunctions::getSuffix(item2));
    btnRdcUnit->setText(rdcUnit+UserfulFunctions::getSuffix(QString("RDC")));

    btnRdcSpeed->setText(this->speed);
    btnRdcRange->setText(this->range);
    this->txtDescription->setText(this->strDescription);
}


void cls6440MeterMode::on_btnMajorUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(this->item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        majorUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnMinorUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(this->item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        minorUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnRdcUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("RDC");
    if(dlg->exec()==QDialog::Accepted)
    {
        rdcUnit = dlg->getSuffix();
        updateButtons();
    }
}
