#include "clsMultiChannelSettings.h"

clsMultiChannelSettings::clsMultiChannelSettings(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void clsMultiChannelSettings::setConditon(int delay, bool isUseLoadData)
{
    txtSwitchDaley->setValue(delay);
    radUserLoad->setChecked(isUseLoadData);
}

int clsMultiChannelSettings::getSwitchDelay()
{
    return txtSwitchDaley->value();
}

bool clsMultiChannelSettings::isUseLoadData()
{
    return radUserLoad->isChecked();
}

void clsMultiChannelSettings::on_btnOk_clicked()
{
    this->accept();
}

void clsMultiChannelSettings::on_btnCancel_clicked()
{
    this->reject();
}
