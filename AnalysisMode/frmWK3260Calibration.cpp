#include "frmWK3260Calibration.h"
#include "MessageDialog.h"
#include "clsRuningSettings.h"
#include "clsSettings.h"
#include "UserfulFunctions.h"
frmWK3260Calibration::frmWK3260Calibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    readSettings();
    //this->tabWidget->setStyleSheet("background-color: rgb(148, 148, 248);");
    myProgressBar->setVisible(false);
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
    strNode ="Calibration-3260/";

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


}
void frmWK3260Calibration::writeSettings()
{
    QString strNode;

    clsSettings settings;
    strNode ="Calibration-3260/";

    settings.writeSetting(strNode+"OpenNormLabel",lblOpenNorm->text());
    settings.writeSetting(strNode+"ShortNormLabel",lblShortNorm->text());
    settings.writeSetting(strNode+"HfNormLabel",lblHFNorm->text());
    settings.writeSetting(strNode+"OpenBoostLabel",lblOpenBoost->text());
    settings.writeSetting(strNode+"ShortBoostLabel",lblShortBoost->text());
    settings.writeSetting(strNode+"HfBoostLabel",lblHFBoost->text());
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

    clsRS::getInst().sendCommand(":CAL:OC-TRIM-P 4");

    this->showProgress(31);

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
    clsRS::getInst().sendCommand(":IMP");
    clsRS::getInst().sendCommand(":IMP:TEST:AC");
}

void frmWK3260Calibration::setCalNorm()
{
    switchToImpMode();
    clsRS::getInst().sendCommand(":IMP:BIAS INT");
    switchToCalMode();
}

void frmWK3260Calibration::setCalBoost()
{
    switchToImpMode();
    clsRS::getInst().sendCommand(":IMP:BIAS EEXT");
    switchToCalMode();
}


void frmWK3260Calibration::on_btnShortNorm_clicked()
{
    PopUpmessage(tr("请保持夹具短路状态"),tr("短路校准"));
    setCalNorm();
    UserfulFunctions::sleepMs(2000);
    clsRS::getInst().sendCommand(":CAL:SC-TRIM-P 4");

    this->showProgress(14);

    lblShortNorm->setText(showInformation(tr("短路"),getCalRes()));

    switchToImpMode();
}

void frmWK3260Calibration::on_btnHFNorm_clicked()
{
    PopUpmessage(tr("请连接高平校准器件"),tr("高频校准"));
    setCalNorm();
    UserfulFunctions::sleepMs(2000);
    clsRS::getInst().sendCommand(":CAL:HF-CAL");
    this->showProgress(37);
    lblHFNorm->setText(showInformation(tr("高频"),getCalRes()));
    switchToImpMode();
}

void frmWK3260Calibration::on_btnOpenBoost_clicked()
{
    PopUpmessage(tr("请保持夹具开路状态"),tr("开路校准"));
    setCalBoost();
    UserfulFunctions::sleepMs(2000);

    clsRS::getInst().sendCommand(":CAL:OC-TRIM-P 4");

    this->showProgress(71);

    lblOpenBoost->setText(showInformation(tr("开路"),getCalRes()));

    switchToImpMode();
}

void frmWK3260Calibration::on_btnShortBoost_clicked()
{
    PopUpmessage(tr("请保持夹具短路状态"),tr("短路校准"));
    setCalBoost();
    UserfulFunctions::sleepMs(2000);
    clsRS::getInst().sendCommand(":CAL:SC-TRIM-P 4");

    this->showProgress(38);

    lblShortBoost->setText(showInformation(tr("短路"),getCalRes()));

    switchToImpMode();
}

void frmWK3260Calibration::on_btnHFBoost_clicked()
{
    PopUpmessage(tr("请连接高频校准器件"),tr("高频校准"));
    setCalBoost();
    UserfulFunctions::sleepMs(2000);
    clsRS::getInst().sendCommand(":CAL:HF-CAL");
    this->showProgress(41);
    lblHFBoost->setText(showInformation(tr("高频"),getCalRes()));
    switchToImpMode();
}
