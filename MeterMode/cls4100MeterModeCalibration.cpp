#include "cls4100MeterModeCalibration.h"

#include "clsRuningSettings.h"
#include <QDebug>
#include "clsSettings.h"
#include "MessageDialog.h"
#include <QDate>
#include <QTime>

cls4100MeterModeCalibration::cls4100MeterModeCalibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    readSettings();
}

void cls4100MeterModeCalibration::readSettings()
{
    clsSettings settings;
    QString strNode =QString("MeterMode-%1/").arg(clsRS::getInst().instrumentModel);

    QString tmp;
    settings.readSetting(strNode + "openSpot",tmp);
    lblOpenSpot->setText(tmp);
    settings.readSetting(strNode + "shortSpot", tmp);
    lblShortSpot->setText(tmp);
    settings.readSetting(strNode + "open10kHz", tmp);
    lblOpen10kHz->setText(tmp);
    settings.readSetting(strNode + "short10kHz",tmp);
    lblShort10kHz->setText(tmp);
    settings.readSetting(strNode + "open100kHz",tmp);
    lblOpen100kHz->setText(tmp);
    settings.readSetting(strNode + "short100kHz",tmp);
    lblShort100kHz->setText(tmp);
    settings.readSetting(strNode + "openAllFreq",tmp);
    lblOpenAllFreq->setText(tmp);
    settings.readSetting(strNode + "shortAllFreq",tmp);
    lblShortAllFreq->setText(tmp);

}

void cls4100MeterModeCalibration::saveSettings()
{
    clsSettings settings;
    QString strNode =QString("MeterMode-%1/").arg(clsRS::getInst().instrumentModel);
    settings.writeSetting(strNode + "openSpot",this->lblOpenSpot->text());
    settings.writeSetting(strNode + "shortSpot", this->lblShortSpot->text());
    settings.writeSetting(strNode + "open10kHz", this->lblOpen10kHz->text());
    settings.writeSetting(strNode + "short10kHz",this->lblShort10kHz->text());
    settings.writeSetting(strNode + "open100kHz",this->lblOpen100kHz->text());
    settings.writeSetting(strNode + "short100kHz",this->lblShort100kHz->text());
    settings.writeSetting(strNode + "openAllFreq",this->lblOpenAllFreq->text());
    settings.writeSetting(strNode + "shortAllFreq",this->lblShortAllFreq->text());

}


bool cls4100MeterModeCalibration::PopUpmessage(QString strMessage,QString strTitle)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ms->setMessage(strMessage,strTitle);
    if(ms->exec())
        return true;
    else
        return false;
}


QString cls4100MeterModeCalibration::openTrim(int type, int delay)
{
    if(!PopUpmessage(tr("移除夹具上的被测物品，然后点击确定！"),tr("开路校准")))
        return "";

    clsRS::getInst().sendCommand(QString(":CAL:OC-TRIM %1").arg(type),false);


    DelaySec(delay);

    if(getCalibrationRes())
        return tr("开路校准成功，校准时间：%1 %2")
                .arg(QDate::currentDate().toString("yyyy-M-dd"))
                .arg(QTime::currentTime().toString("hh:mm:ss"));
    else
        return tr("开路校准失败。");
    saveSettings();
}

QString cls4100MeterModeCalibration::shortTrim(int type, int delay)
{
    if(! PopUpmessage(tr("请将夹具短路，然后点击确定"),tr("短路校准")))
        return "";

    clsRS::getInst().sendCommand(QString(":CAL:SC-TRIM %1").arg(type),false);
    DelaySec(delay);
    if(getCalibrationRes())
        return (tr("短路校准成功。校准时间：%1 %2")
                .arg(QDate::currentDate().toString("yyyy-M-dd"))
                .arg(QTime::currentTime().toString("hh:mm:ss")));
    else
        return (tr("短路校准失败。"));
    saveSettings();
}




void cls4100MeterModeCalibration::Increase()
{
    int i = dial->value();

    int intIncreasement = (dial->maximum()-dial->minimum())/9;

    dial->setValue((i+intIncreasement)%dial->maximum());
    QApplication::processEvents();
}

void cls4100MeterModeCalibration::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}



bool cls4100MeterModeCalibration::getCalibrationRes()
{
    QString calRes = clsRS::getInst().sendCommand(":CAL:RES?",true);

    if(calRes.toInt()==1)
        return true;
    else
        return false;
}

void cls4100MeterModeCalibration::DelaySec(int value)
{
    QTime time1 = QTime::currentTime();

    while(time1.secsTo(QTime::currentTime())<=value)
    {
        Increase();
        qApp->processEvents();
        this->sleepMs(500);
    }
}

void cls4100MeterModeCalibration::on_btnOpenSpot_clicked()
{
    lblOpenSpot->setText(openTrim(1,4));
}

void cls4100MeterModeCalibration::on_btnOpen10kHz_clicked()
{
    lblOpen10kHz->setText(openTrim(2,22));
}

void cls4100MeterModeCalibration::on_btnOpen100kHz_clicked()
{
    lblOpen100kHz->setText(openTrim(3,35));
}

void cls4100MeterModeCalibration::on_btnOpenAllFreq_clicked()
{
    lblOpenAllFreq->setText(openTrim(4,52));
}

void cls4100MeterModeCalibration::on_btnShortSpot_clicked()
{
    lblShortSpot->setText(shortTrim(1,8));
}

void cls4100MeterModeCalibration::on_btnShort10kHz_clicked()
{
    lblShort10kHz->setText(shortTrim(2,22));
}

void cls4100MeterModeCalibration::on_btnShort100kHZ_clicked()
{
    lblShort100kHz->setText(shortTrim(3,34));
}

void cls4100MeterModeCalibration::on_btnShortAllFreq_clicked()
{
    lblShortAllFreq->setText(shortTrim(4,48));
}

void cls4100MeterModeCalibration::on_btnClose_clicked()
{
    saveSettings();
    this->close();
}
