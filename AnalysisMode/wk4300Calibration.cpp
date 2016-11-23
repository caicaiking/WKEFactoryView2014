#include "wk4300Calibration.h"
#include "clsRuningSettings.h"
#include <QDebug>
#include "UserfulFunctions.h"
#include "MessageDialog.h"
#include "clsSettings.h"
wk4300Calibration::wk4300Calibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    myProgressBar->setVisible(false);
    readSettings();
}


void wk4300Calibration::readSettings()
{
    QString strNode;
    clsSettings settings;
    QString strTemp;
    strNode ="Calibration-4300/";

    settings.readSetting(strNode+"openLabel",strTemp);
    lblOpen->setText(strTemp);
    settings.readSetting(strNode+"ShortLabel",strTemp);
    lblShort->setText(strTemp);


}
void wk4300Calibration::writeSettings()
{
    QString strNode;

    clsSettings settings;
    strNode ="Calibration-4300/";

    settings.writeSetting(strNode+"openLabel",lblOpen->text());
    settings.writeSetting(strNode+"ShortLabel",lblShort->text());

}


void wk4300Calibration::on_btnOpen_clicked()
{
    PopUpmessage(tr("移除夹具上的被测物品，然后点击确定！"),tr("开路校准"));

    clsRS::getInst().sendCommand(":CAL:OC-TRIM 4",false);
    showProgress(0);
}
void wk4300Calibration::PopUpmessage(QString strMessage,QString strTitle)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ms->setMessage(strMessage,strTitle);
    ms->exec();
}

void wk4300Calibration::showProgress(int i)
{
    myProgressBar->setVisible(true);
    if(i==0)
    {
        myProgressBar->setMaximum(52);
    }
    else
    {
        myProgressBar->setMaximum(49);
    }

    myProgressBar->setValue(0);

    while(myProgressBar->value()<myProgressBar->maximum())
    {
        UserfulFunctions::sleepMs(1000);
        myProgressBar->setValue(myProgressBar->value()+1);
    }

    if(clsRS::getInst().getConnectionType().toUpper()=="USB")
    {
        UserfulFunctions::sleepMs(2000);
        clsRS::getInst().disConnect();
    }


    QString calRes = clsRS::getInst().sendCommand(":CAL:RES?",true);

    if(calRes.toInt()==1)
    {
        if(i==0)
        {
            lblOpen->setText(tr("开路校准成功，校准时间：%1 %2")
                             .arg(QDate::currentDate().toString("yyyy-M-dd"))
                             .arg(QTime::currentTime().toString("hh:mm:ss")));
        }
        else
        {
            lblShort->setText(tr("短路校准成功。校准时间：%1 %2")
                              .arg(QDate::currentDate().toString("yyyy-M-dd"))
                              .arg(QTime::currentTime().toString("hh:mm:ss")));
        }
    }
    else
    {
        if(i==0)
        {
            lblOpen->setText(tr("开路校准失败。"));
        }
        else
        {
            lblShort->setText(tr("短路校准失败。"));
        }
    }

    myProgressBar->setVisible(false);
}

void wk4300Calibration::on_btnOk_clicked()
{
    this->writeSettings();
    this->accept();
}

void wk4300Calibration::on_btnShort_clicked()
{
    PopUpmessage(tr("请将夹具短路，然后点击确定"),tr("短路校准"));
    clsRS::getInst().sendCommand(":CAL:SC-TRIM 4",false);
    showProgress(1);
}
