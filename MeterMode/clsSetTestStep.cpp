#include "clsSetTestStep.h"
#include "clsMeterModeFactory.h"
#include "clsRuningSettings.h"
#include <QLayout>
#include <QBoxLayout>
clsSetTestStep::clsSetTestStep(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    sngTestDisplay->setMessage(tr("没有数据可以显示"),0);
    meter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(meter);
    meterWidget->setLayout(layout);
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
