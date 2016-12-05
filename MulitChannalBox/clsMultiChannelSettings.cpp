#include "clsMultiChannelSettings.h"

clsMultiChannelSettings::clsMultiChannelSettings(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void clsMultiChannelSettings::setCondition(int delay, bool isUseLoadData)
{
    txtSwitchDaley->setValue(delay);
    radUserLoad->setChecked(isUseLoadData);
    rbRCLoad->setChecked(!isUseLoadData);
}

void clsMultiChannelSettings::setCondition(bool isFastGpibOn)
{
    this->rbTurnOnScreen->setChecked(!isFastGpibOn);
    this->rbTurnOffScreen->setChecked(isFastGpibOn);
}

int clsMultiChannelSettings::getSwitchDelay()
{
    return txtSwitchDaley->value();
}

bool clsMultiChannelSettings::isUseLoadData()
{
    return radUserLoad->isChecked();
}

bool clsMultiChannelSettings::getTurnOnScreen()
{
    return this->rbTurnOffScreen->isChecked();
}

void clsMultiChannelSettings::on_btnOk_clicked()
{
    this->accept();
}

void clsMultiChannelSettings::setLedLightEnable(bool value)
{
    chkUseLedLight->setChecked(!value);
}

bool clsMultiChannelSettings::getLedEnable()
{
    return !this->chkUseLedLight->isChecked();
}

void clsMultiChannelSettings::on_btnCancel_clicked()
{
    this->reject();
}
