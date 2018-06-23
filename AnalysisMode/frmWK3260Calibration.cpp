#include "frmWK3260Calibration.h"
#include "MessageDialog.h"
#include "clsRuningSettings.h"
#include "clsSettings.h"
#include "UserfulFunctions.h"
frmWK3260Calibration::frmWK3260Calibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    readSettings();
    //this->tabWidget->setStyleSheet("background-color: rgb(148, 148, 248);");
    myProgressBar->setVisible(false);

    tabSettingTime.reset(tabWidget->widget(2));
    tabWidget->removeTab(2);


}

void frmWK3260Calibration::PopUpmessage(QString strMessage,QString strTitle)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ms->setMessage(strMessage,strTitle);
    ms->exec();
}


void frmWK3260Calibration::readSettings()
{
    QString strNode;
    clsSettings settings;
    QString strTemp;
    strNode =QString("Calibration-%1/").arg(clsRS::getInst().meterSeries);

    settings.readSetting(strNode+"OpenNormLabel",strTemp);
    lblOpenNorm->setText(strTemp);
    settings.readSetting(strNode+"ShortNormLabel",strTemp);
    lblShortNorm->setText(strTemp);
    settings.readSetting(strNode+"HfNormLabel",strTemp);
    lblHFNorm->setText(strTemp);
    settings.readSetting(strNode+"OpenBoostLabel",strTemp);
    lblOpenBoost->setText(strTemp);
    settings.readSetting(strNode+"ShortBoostLabel",strTemp);
    lblShortBoost->setText(strTemp);
    settings.readSetting(strNode+"HfBoostLabel",strTemp);
    lblHFBoost->setText(strTemp);

    settings.readSetting(strNode+"normOpenTime", this->normOpenTime);
    if(this->normOpenTime<10)
        this->normOpenTime = 31;

    settings.readSetting(strNode+"normShortTime", this->normShortTime);
    if(this->normShortTime < 10)
        this->normShortTime = 14;

    settings.readSetting(strNode+"normHFTime", this->normHFTime);
    if(this->normHFTime<10)
        this->normHFTime =37;

    settings.readSetting(strNode+"boostOpenTime", this->boostOpenTime);
    if(this->boostOpenTime<10)
        this->boostOpenTime =71;

    settings.readSetting(strNode+"boostShortTime", this->boostShortTime);
    if(this->boostShortTime<10)
        this->boostShortTime =38;

    settings.readSetting(strNode+"boostHFTime", this->boostHFTime);
    if(this->boostHFTime<10)
        this->boostHFTime =66;

    this->txtNormOpen->setValue(this->normOpenTime);
    this->txtNormShort->setValue(this->normShortTime);
    this->txtNormHF->setValue(this->normHFTime);
    this->txtBoostOpen->setValue(this->boostOpenTime);
    this->txtBoostShort->setValue(this->boostShortTime);
    this->txtBoostHF->setValue(this->boostHFTime);



}
void frmWK3260Calibration::writeSettings()
{
    QString strNode;

    clsSettings settings;
    strNode =QString("Calibration-%1/").arg(clsRS::getInst().meterSeries);

    settings.writeSetting(strNode+"OpenNormLabel",lblOpenNorm->text());
    settings.writeSetting(strNode+"ShortNormLabel",lblShortNorm->text());
    settings.writeSetting(strNode+"HfNormLabel",lblHFNorm->text());
    settings.writeSetting(strNode+"OpenBoostLabel",lblOpenBoost->text());
    settings.writeSetting(strNode+"ShortBoostLabel",lblShortBoost->text());
    settings.writeSetting(strNode+"HfBoostLabel",lblHFBoost->text());

    settings.writeSetting(strNode+"normOpenTime", this->normOpenTime);
    settings.writeSetting(strNode+"normShortTime", this->normShortTime);
    settings.writeSetting(strNode+"normHFTime", this->normHFTime);

    settings.writeSetting(strNode+"boostOpenTime", this->boostOpenTime);
    settings.writeSetting(strNode+"boostShortTime", this->boostShortTime);
    settings.writeSetting(strNode+"boostHFTime", this->boostHFTime);
}

void frmWK3260Calibration::on_btnOk_clicked()
{
    writeSettings();
    this->accept();
}

void frmWK3260Calibration::showProgress(int delayTime)
{
    myProgressBar->setVisible(true);
    myProgressBar->setMaximum(delayTime);
    myProgressBar->setValue(0);

    while(myProgressBar->value()<myProgressBar->maximum())
    {
        UserfulFunctions::sleepMs(1000);
        myProgressBar->setValue(myProgressBar->value()+1);
    }

    myProgressBar->setVisible(false);
}

QString frmWK3260Calibration::showInformation(QString calType, int calRes)
{
    QString tmp;
    if(calRes==1)
    {
        tmp=QString(tr("%1校准成功，校准时间：%2 %3")
                    .arg(calType)
                    .arg(QDate::currentDate().toString("yyyy-M-dd"))
                    .arg(QTime::currentTime().toString("hh:mm:ss")));
    }
    else
    {
        tmp=QString(tr("%1校准失败").arg(calType));
    }

    return tmp;
}

void frmWK3260Calibration::on_btnOpenNorm_clicked()
{
    PopUpmessage(tr("请保持夹具开路状态"),tr("开路校准"));
    setCalNorm();

    if(clsRS::getInst().meterSeries=="3260")
        clsRS::getInst().sendCommand(":CAL:OC-TRIM-P 4");
    else
        clsRS::getInst().sendCommand(":CAL:OC-TRIM 2");

    this->showProgress(this->normOpenTime);

    lblOpenNorm->setText(showInformation(tr("开路"),getCalRes()));

    switchToImpMode();
}

int frmWK3260Calibration::getCalRes()
{
    QString ret=clsRS::getInst().sendCommand(":CAL:RES?",true);
    return  ret.toDouble();
}

void frmWK3260Calibration::switchToCalMode()
{
    clsRS::getInst().sendCommand(":CAL");
}

void frmWK3260Calibration::switchToImpMode()
{
    clsRS::getInst().sendCommand(getGpibMeter());
    clsRS::getInst().sendCommand(getGpibMeter()+":TEST:AC");
}

void frmWK3260Calibration::setCalNorm()
{
    switchToImpMode();
    clsRS::getInst().sendCommand(getGpibMeter()+":BIAS INT");
    switchToCalMode();
}

void frmWK3260Calibration::setCalBoost()
{
    switchToImpMode();
    clsRS::getInst().sendCommand(getGpibMeter()+":BIAS EEXT");
    switchToCalMode();
}


void frmWK3260Calibration::on_btnShortNorm_clicked()
{
    PopUpmessage(tr("请保持夹具短路状态"),tr("短路校准"));
    setCalNorm();
    UserfulFunctions::sleepMs(2000);
    if(clsRS::getInst().meterSeries=="3260")
        clsRS::getInst().sendCommand(":CAL:SC-TRIM-P 4");
    else
        clsRS::getInst().sendCommand(":CAL:SC-TRIM 2");
    this->showProgress(this->normShortTime);

    lblShortNorm->setText(showInformation(tr("短路"),getCalRes()));

    switchToImpMode();
}

void frmWK3260Calibration::on_btnHFNorm_clicked()
{
    PopUpmessage(tr("请连接高频校准器件"),tr("高频校准"));
    setCalNorm();
    UserfulFunctions::sleepMs(2000);
    clsRS::getInst().sendCommand(":CAL:HF-CAL");
    this->showProgress(this->normHFTime);
    lblHFNorm->setText(showInformation(tr("高频"),getCalRes()));
    switchToImpMode();
}

void frmWK3260Calibration::on_btnOpenBoost_clicked()
{
    PopUpmessage(tr("请保持夹具开路状态"),tr("开路校准"));
    setCalBoost();
    UserfulFunctions::sleepMs(2000);

    if(clsRS::getInst().meterSeries=="3260")
        clsRS::getInst().sendCommand(":CAL:OC-TRIM-P 4");
    else
        clsRS::getInst().sendCommand(":CAL:OC-TRIM 2");
    this->showProgress(this->boostOpenTime);

    lblOpenBoost->setText(showInformation(tr("开路"),getCalRes()));

    switchToImpMode();
}

void frmWK3260Calibration::on_btnShortBoost_clicked()
{
    PopUpmessage(tr("请保持夹具短路状态"),tr("短路校准"));
    setCalBoost();
    UserfulFunctions::sleepMs(2000);

    if(clsRS::getInst().meterSeries=="3260")
        clsRS::getInst().sendCommand(":CAL:SC-TRIM-P 4");
    else
        clsRS::getInst().sendCommand(":CAL:SC-TRIM 2");

    this->showProgress(this->boostShortTime);

    lblShortBoost->setText(showInformation(tr("短路"),getCalRes()));

    switchToImpMode();
}

void frmWK3260Calibration::on_btnHFBoost_clicked()
{
    PopUpmessage(tr("请连接高频校准器件"),tr("高频校准"));
    setCalBoost();
    UserfulFunctions::sleepMs(2000);
    clsRS::getInst().sendCommand(":CAL:HF-CAL");
    this->showProgress(this->boostHFTime);
    lblHFBoost->setText(showInformation(tr("高频"),getCalRes()));
    switchToImpMode();
}
QString frmWK3260Calibration::getGpibMeter()
{
    if(clsRS::getInst().meterSeries=="3260")
    {
        return ":IMP";
    }
    else
    {
        return ":MEAS";
    }
}

void frmWK3260Calibration::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() == Qt::ControlModifier && e->key()==Qt::Key_9)
    {
        if(tabWidget->count()==2)
        {
            tabWidget->insertTab(2,tabSettingTime.data(),tr("校准延时设置"));
        }
        else
        {
            tabWidget->removeTab(2);
        }
    }
}

void frmWK3260Calibration::on_btnSaveNormTime_clicked()
{
    this->normOpenTime = txtNormOpen->value();
    this->normShortTime = txtNormShort->value();
    this->normHFTime = txtNormHF->value();

    writeSettings();
}

void frmWK3260Calibration::on_btnSaveBoostTime_clicked()
{
    this->boostOpenTime = txtBoostOpen->value();
    this->boostShortTime = txtBoostShort->value();
    this->boostHFTime = txtBoostHF->value();

    writeSettings();

}
