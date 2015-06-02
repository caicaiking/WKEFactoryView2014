#include "clsSetTestStep.h"
#include "clsMeterModeFactory.h"
#include "clsRuningSettings.h"
#include <QLayout>
#include <QBoxLayout>
#include <QDebug>

clsSetTestStep::clsSetTestStep(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    sngTestDisplay->setMessage(tr("没有数据可以显示"),0);
    meter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(meter);

    connect(meter,SIGNAL(biasStatus(bool)),this,SLOT(biasSlot(bool)));
    meterWidget->setLayout(layout);

    connect(meter,SIGNAL(signalTestResult(QString)),this,SLOT(showRes(QString)));
}




void clsSetTestStep::showRes(QString value)
{

    sngTestDisplay->setMessage(value,meter->getCountTestItems());
}

void clsSetTestStep::biasSlot(bool value)
{
    if(!value)
        btnTurnOffBias->setIcon(QIcon(":/Icons/BiasOff.png"));
    else
        btnTurnOffBias->setIcon(QIcon(":/Icons/BiasOn.png"));
}

void clsSetTestStep::setTestSteps(QList<WKEMeterMode *> value)
{

}

QList<WKEMeterMode *> clsSetTestStep::getTestSteps()
{

}

void clsSetTestStep::on_btnOk_clicked()
{
    this->accept();
}

void clsSetTestStep::on_btnCancel_clicked()
{
    this->reject();
}



void clsSetTestStep::on_btnTurnOffBias_clicked()
{
    meter->turnOffBias();
    biasSlot(false);
}

void clsSetTestStep::on_btnTest_clicked()
{
    sngTestDisplay->setMessage(tr("更新测试条件"),0);
    meter->updateGPIB();
    sngTestDisplay->setMessage(tr("正在测试"),0);
    meter->singleTrig();
}


void clsSetTestStep::on_btnNewStep_clicked()
{
    step = meter->getConditon();
}

void clsSetTestStep::on_btnSave_clicked()
{
    meter->setCondition(this->step);
}
