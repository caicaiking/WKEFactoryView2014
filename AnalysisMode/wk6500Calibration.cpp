#include "wk6500Calibration.h"
#include "clsConnection.h"
#include <QDebug>
#include <doubleType.h>
#include <QObject>
#include <NumberInput.h>
#include "clsSettings.h"
#include "MessageDialog.h"
#include <QBoxLayout>
#include "clsRuningSettings.h"
#include "clsRuningSettings.h"
wk6500Calibration::wk6500Calibration(double maxFreq, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
 setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    dblMaxFrequency = maxFreq;

    readSettings();
    if(dblCalibrateFrequecy==0.0)
    {

        doubleType dt(dblMaxFrequency,"");

        btnSetLimit->setText(dt.formateToString(6) +"Hz");
    }
    else
    {
        if(dblMaxFrequency< dblCalibrateFrequecy)
            dblCalibrateFrequecy =dblMaxFrequency;

        doubleType dt(dblCalibrateFrequecy,"");
        btnSetLimit->setText(dt.formateToString(6) +"Hz");
    }
}

void wk6500Calibration::PopUpmessage(QString strMessage,QString strTitle)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ms->setMessage(strMessage,strTitle);
    ms->exec();
}

void wk6500Calibration::setMaxFreq(double value)
{
    this->dblMaxFrequency = value;
}


void wk6500Calibration::readSettings()
{
    QString strNode;
    clsSettings settings;
    QString strTemp;
    strNode ="Calibration-6500/";
    settings.readSetting(strNode+"CalFrequency", dblCalibrateFrequecy);
    settings.readSetting(strNode+"openLabel",strTemp);
    lblOpen->setText(strTemp);
    settings.readSetting(strNode+"ShortLabel",strTemp);
    lblShort->setText(strTemp);

    settings.writeSetting(strNode+"HFLable",strTemp);
    lblHF->setText(strTemp);
}
void wk6500Calibration::writeSettings()
{
    QString strNode;

    clsSettings settings;
    strNode ="Calibration-6500/";
    settings.writeSetting(strNode+"CalFrequency", dblCalibrateFrequecy);
    settings.writeSetting(strNode+"openLabel",lblOpen->text());
    settings.writeSetting(strNode+"ShortLabel",lblShort->text());
    settings.writeSetting(strNode+"HFLable",lblHF->text());
}


QString wk6500Calibration::GetModel()
{

    if(!clsRS::getInst().isConnectionInit())
        return "";

    QString strMode =clsRS::getInst().sendCommand("*IDN?",true);

    if (strMode.isEmpty())
        return "";

    QStringList strModeInfo = strMode.split(",");

    if(strModeInfo.count()<2)
        return "";

    strMode = strModeInfo[1];

    strMode = strMode.trimmed();
    return strMode;
}

double wk6500Calibration::GetMaxFrequency()
{
    QString mode = GetModel();

    if(mode.contains("6505"))
        return 5e6;
    else if(mode.contains("6510"))
        return 10e6;
    else if(mode.contains("6520"))
        return 20e6;
    else if(mode.contains("6515"))
        return 15e6;
    else if(mode.contains("6530"))
        return 30e6;
    else if(mode.contains("6550"))
        return 50e6;
    else if(mode.contains("65120"))
        return 120e6;
    else
        return 0.0;

    return 0;
}

void wk6500Calibration::on_btnOpen_clicked()
{
    QString strOpenCommand =":CAL:OC-TRIM2";
    QString strStatusCommand="*STATUS?";

    PopUpmessage(tr("移除夹具上的被测物品，然后点击确定！"),tr("开路校准"));

    clsRS::getInst().sendCommand(strOpenCommand,false);

    sleepMs(2000);
    Increase();

    int i=clsRS::getInst().sendCommand(strStatusCommand,true).toInt();
    sleepMs(2000);
    Increase();

    i=clsRS::getInst().sendCommand(strStatusCommand,true).toInt();
    while(i==11  || i==1)
    {
        sleepMs(100);
        QString strI=clsRS::getInst().sendCommand(strStatusCommand,true);

        i= strI.toInt();

        Increase();
       // qDebug()<<"校准返回值："<<i;
    }

    if(i<64)
    {
        lblOpen->setText(tr("开路校准成功，频率至%1。\n校准时间：%2 %3")
                         .arg(btnSetLimit->text())
                         .arg(QDate::currentDate().toString("yyyy-M-dd"))
                         .arg(QTime::currentTime().toString("hh:mm:ss")));

        clsRS::getInst().sendCommand(":CAL:STORE-TRIMS",false);
    }
    else
    {
        lblOpen->setText(tr("开路校准失败。"));
    }
    writeSettings();
}

void wk6500Calibration::Increase()
{
    int i = dial->value();

    int intIncreasement = (dial->maximum()-dial->minimum())/9;

    dial->setValue((i+intIncreasement)%dial->maximum());
    QApplication::processEvents();
}

void wk6500Calibration::sleepMs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void wk6500Calibration::on_btnSetLimit_clicked()
{
    NumberInput wk(this);
    wk.setWindowTitle(tr("选择校准频率"));

    if(wk.exec()==QDialog::Accepted)
    {
        QString unit;
        double value;


        value =wk.getValue();
        unit = wk.getSuffix();
        doubleType dt(value, unit);
        dblCalibrateFrequecy = dt.Data();
        if(dt.Data()<1e6)
            dblCalibrateFrequecy =1E6;
        if(dt.Data() > dblMaxFrequency)
            dblCalibrateFrequecy = dblMaxFrequency;

        dt.setData(dblCalibrateFrequecy,"");
        btnSetLimit->setText(dt.formateToString(6)+"Hz");

        QString strCommand = ":CAL:FREQ-LIMIT " + QString::number(dblCalibrateFrequecy);
        clsRS::getInst().sendCommand(strCommand,true);

    }
    writeSettings();
}

void wk6500Calibration::on_btnDone_clicked()
{
    writeSettings();
    this->accept();
}


void wk6500Calibration::on_btnClearOpenShort_clicked()
{
    clsRS::getInst().sendCommand(":CAL:CLEAR-TRIMS",false);

    lblOpen->setText(tr("工厂默认值。"));
    lblShort->setText(tr("工厂默认值。"));
    writeSettings();
}

void wk6500Calibration::on_btnShort_clicked()
{
    QString strOpenCommand =":CAL:SC-TRIM2";
    QString strStatusCommand="*STATUS?";

    PopUpmessage(tr("请将夹具短路，然后点击确定"),tr("短路校准"));

    clsRS::getInst().sendCommand(strOpenCommand,false);

    Increase();
    sleepMs(2000);
    Increase();

    int i=clsRS::getInst().sendCommand(strStatusCommand,true).toInt();
    sleepMs(2000);
    Increase();

    i=clsRS::getInst().sendCommand(strStatusCommand,true).toInt();
    while(i==11   || i==1)
    {
        sleepMs(100);
        QString strI=clsRS::getInst().sendCommand(strStatusCommand,true);

        i= strI.toInt();
        Increase();
    }

    if(i<64)
    {
        lblShort->setText(tr("短路校准成功，频率至%1。\n校准时间：%2 %3")
                          .arg(btnSetLimit->text())
                          .arg(QDate::currentDate().toString("yyyy-M-dd"))
                          .arg(QTime::currentTime().toString("hh:mm:ss")));

        clsRS::getInst().sendCommand(":CAL:STORE-TRIMS",false);
    }
    else
    {
        lblShort->setText(tr("短路校准失败。"));
    }
    writeSettings();
}

void wk6500Calibration::on_btnHF_clicked()
{
    QString strCommand=":CAL:HF-COMP";
    QString strContinu ="*CAL-CONTINUE";
    QString strStatus ="*STATUS?";

    clsRS::getInst().sendCommand(strCommand);

    sleepMs(2000);
    Increase();

    int i=clsRS::getInst().sendCommand(strStatus,true).toInt();

    while(i!=3)
    {
        sleepMs(100);
        i=clsRS::getInst().sendCommand(strStatus,true).toInt();
    }

    PopUpmessage(tr("请在夹具上连接100R电阻"),tr("100R 电阻"));

    clsRS::getInst().sendCommand(strContinu);
    while(i!=5)
    {
        Increase();
        sleepMs(100);
        i=clsRS::getInst().sendCommand(strStatus,true).toInt();
    }
    PopUpmessage(tr("请在夹具上连接100pF电容"),tr("100pF 电容"));

    clsRS::getInst().sendCommand(strContinu);
    sleepMs(2000);
    Increase();

    while(1)
    {
        Increase();
        sleepMs(100);
        i=clsRS::getInst().sendCommand(strStatus,true).toInt();
        if(i==10  || i==0)
        {
            lblHF->setText(tr("高频校准成功，频率至%1。\n校准时间：%2 %3")
                           .arg(btnSetLimit->text())
                           .arg(QDate::currentDate().toString("yyyy-M-dd"))
                           .arg(QTime::currentTime().toString("hh:mm:ss")));
            clsRS::getInst().sendCommand(":CAL:STORE-HF-COMP");
            return;
        }
        else if(i==180 || i==128)
        {
            lblHF->setText(tr("高频校准失败，使用工厂默认值！"));
            clsRS::getInst().sendCommand(":CAL:CLEAR-HF-COMP");
            return;
        }
        else if(i==19  || i==9)
        {
            PopUpmessage(tr("请在夹具上连接10pF电容"),tr("10pF 电容"));
            clsRS::getInst().sendCommand(strContinu);
            sleepMs(2000);
        }
    }
    writeSettings();
}

void wk6500Calibration::on_btnClearHF_clicked()
{
    clsRS::getInst().sendCommand(":CAL:CLEAR-HF-COMP");
    lblHF->setText(tr("工厂默认值。"));
    writeSettings();
}
