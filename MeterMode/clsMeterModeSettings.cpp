#include "clsMeterModeSettings.h"

clsMeterModeSettings::clsMeterModeSettings(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void clsMeterModeSettings::setCondition(const MeterSettings &st)
{
    txtPreDelay->setValue(st.preDelay);
    txtLastDelay->setValue(st.lastDelay);

    switch (st.trigMode) {
    case SWTrig:
        rbSoftWare->setChecked(true);
        break;
    case Adu200Trig:
        rbAdu200->setChecked(true);
        break;
    case AutoDetectTrig:
        rbAutoDetect->setChecked(true);
        break;
    default:
        break;
    }

    switch (st.saveResType)
    {
    case  AllRes:
        rbAll->setChecked(true);
        break;
    case PassRes:
        rbPass->setChecked(true);
        break;
    case FailRes:
        rbFail->setChecked(true);
        break;
    default:
        break;
    }
}

MeterSettings clsMeterModeSettings::getCondtion()
{
    MeterSettings tmp;
    tmp.preDelay = txtPreDelay->value();
    tmp.lastDelay = txtLastDelay->value();
    if(rbSoftWare->isChecked())
        tmp.trigMode = SWTrig;

    if(rbAdu200->isChecked())
        tmp.trigMode = Adu200Trig;

    if(rbAutoDetect->isChecked())
        tmp.trigMode = AutoDetectTrig;

    if(rbAll->isChecked())
        tmp.saveResType = AllRes;

    if(rbPass->isChecked())
        tmp.saveResType = PassRes;

    if(rbFail->isChecked())
        tmp.saveResType = FailRes;

    return tmp;
}

void clsMeterModeSettings::on_btnOk_clicked()
{
    this->accept();
}

void clsMeterModeSettings::on_btnCancel_clicked()
{
    this->reject();
}
