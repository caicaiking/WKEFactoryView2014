#include "clsBiasAOperation.h"
#include "clsSettings.h"
#include "UserfulFunctions.h"
clsBiasAOperation::clsBiasAOperation(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    readSettings();
}

void clsBiasAOperation::updateButtion()
{
    grpPreDelay->setChecked(blPreBias);
    grpAfterDelay->setChecked(blAfterBias);

    txtPreDelay->setValue(this->preValue);
    txtAfterDelay->setValue(this->afterValue);
}

void clsBiasAOperation::on_btnOk_clicked()
{
    blPreBias = this->grpPreDelay->isChecked();
    blAfterBias = this->grpAfterDelay->isChecked();

    preValue = txtPreDelay->value();
    afterValue = txtAfterDelay->value();

    writeSettings();
    this->accept();
}

void clsBiasAOperation::on_btnCancel_clicked()
{
    this->reject();
}

void clsBiasAOperation::on_btnReset_clicked()
{
    grpPreDelay->setChecked(false);
    grpAfterDelay->setChecked(false);
    txtPreDelay->setValue(0);
    txtAfterDelay->setValue(0);
}

void clsBiasAOperation::writeSettings()
{
    clsSettings settings;

    QString strNode = "BiasASettings/";

    settings.writeSetting(strNode +"PreDelayEnable",this->blPreBias);
    settings.writeSetting(strNode +"PreDelayBias",this->preValue);
    settings.writeSetting(strNode +"AfterDelayEnabel",this->blAfterBias);
    settings.writeSetting(strNode+"AfterDelayBias",this->afterValue);
}

void clsBiasAOperation::readSettings()
{
    clsSettings settings;

    QString strNode = "BiasASettings/";

    settings.readSetting(strNode +"PreDelayEnable",this->blPreBias);

    settings.readSetting(strNode +"PreDelayBias",this->preValue);
    if(this->preValue>txtPreDelay->maximum() || this->preValue<txtPreDelay->minimum())
        this->preValue=0;

    settings.readSetting(strNode +"AfterDelayEnabel",this->blAfterBias);

    settings.readSetting(strNode+"AfterDelayBias",this->afterValue);

    if(this->afterValue>txtAfterDelay->maximum() || this->afterValue<txtAfterDelay->minimum())
        this->txtAfterDelay=0;
    updateButtion();
}
