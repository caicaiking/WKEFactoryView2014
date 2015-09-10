#include "wk6440Calibration.h"
#include "clsSettings.h"
#include <QTime>
#include "clsRuningSettings.h"
#include "MessageDialog.h"
wk6440Calibration::wk6440Calibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
 setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    if(clsRS::getInst().instrumentModel=="6430")
    {
        btnHF->setEnabled(false);
    }

    clsRS::getInst().sendCommand(":CAL");

}
void wk6440Calibration::PopUpmessage(QString strMessage,QString strTitle)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ms->setMessage(strMessage,strTitle);
    ms->exec();
}


void wk6440Calibration::readSettings()
{
    QString strNode;
    clsSettings settings;
    QString strTemp;

    strNode ="Calibration-6440/";
    settings.readSetting(strNode+"openLabel",strTemp);
    lblOpen->setText(strTemp);
    settings.readSetting(strNode+"ShortLabel",strTemp);
    lblShort->setText(strTemp);

    settings.writeSetting(strNode+"HFLable",strTemp);
    lblHF->setText(strTemp);
}
void wk6440Calibration::writeSettings()
{
    QString strNode;
    clsSettings settings;

    strNode ="Calibration-6440/";
    settings.writeSetting(strNode+"openLabel",lblOpen->text());
    settings.writeSetting(strNode+"ShortLabel",lblShort->text());
    settings.writeSetting(strNode+"HFLable",lblHF->text());
}

void wk6440Calibration::Increase()
{
    int i = dial->value();

    int intIncreasement = (dial->maximum()-dial->minimum())/9;

    dial->setValue((i+intIncreasement)%dial->maximum());
    QApplication::processEvents();
}

void wk6440Calibration::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void wk6440Calibration::on_btnDone_clicked()
{
    writeSettings();
    clsRS::getInst().sendCommand(":MEAS;:MEAS:TEST;:MEAS:TEST:AC");
    this->accept();
}

bool wk6440Calibration::getCalibrationRes()
{
    QString calRes = clsRS::getInst().sendCommand(":CAL:RES?",true);

    if(calRes.toInt()==1)
        return true;
    else
        return false;
}

void wk6440Calibration::DelaySec(int value)
{
    QTime time1 = QTime::currentTime();

    while(time1.secsTo(QTime::currentTime())<=value)
    {
        Increase();
        qApp->processEvents();
        this->sleepMs(500);
    }
}

void wk6440Calibration::on_btnOpen_clicked()
{
    PopUpmessage(tr("移除夹具上的被测物品，然后点击确定！"),tr("开路校准"));

    clsRS::getInst().sendCommand(":CAL:OC-TRIM 4",false);


    DelaySec(35);

    if(getCalibrationRes())
        lblOpen->setText(tr("开路校准成功，校准时间：%1 %2")
                         .arg(QDate::currentDate().toString("yyyy-M-dd"))
                         .arg(QTime::currentTime().toString("hh:mm:ss")));
    else
        lblOpen->setText(tr("开路校准失败。"));
}

void wk6440Calibration::on_btnShort_clicked()
{
    PopUpmessage(tr("请将夹具短路，然后点击确定"),tr("短路校准"));
    clsRS::getInst().sendCommand(":CAL:SC-TRIM 4",false);
    DelaySec(20);
    if(getCalibrationRes())
        lblShort->setText(tr("短路校准成功。校准时间：%1 %2")
                          .arg(QDate::currentDate().toString("yyyy-M-dd"))
                          .arg(QTime::currentTime().toString("hh:mm:ss")));
    else
        lblShort->setText(tr("短路校准失败。"));
}

void wk6440Calibration::on_btnHF_clicked()
{
    PopUpmessage(tr("请连接高频校准器件"),tr("高频校准"));
    clsRS::getInst().sendCommand(":CAL:HF-CAL");
    DelaySec(60);
    if(getCalibrationRes())
        lblHF->setText(tr("高频校准成功。校准时间：%1 %2")
                       .arg(QDate::currentDate().toString("yyyy-M-dd"))
                       .arg(QTime::currentTime().toString("hh:mm:ss")));
    else
        lblHF->setText(tr("高频校准失败。"));

}
