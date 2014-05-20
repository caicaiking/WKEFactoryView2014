#include "frmWk3260.h"
#include "Globle.h"
#include "clsRuningSettings.h"
#include "QDebug"
#include "dlgLevelInputBox.h"
#include "NumberInput.h"
#include "clsSettings.h"
#include "dlgSpeed.h"
#include "UserfulFunctions.h"
#include "frmWK3260Calibration.h"
frmWk3260::frmWk3260(WKEInstrument *parent) :
    WKEInstrument(parent)
{
    setupUi(this);
    readSettings(this->wk3260);
    updateButtons();

    QString Meter = ":IMP";
    clsRS::getInst().sendCommand(Meter);
    clsRS::getInst().sendCommand(Meter+":TEST");
    clsRS::getInst().sendCommand(Meter+":TEST:AC");
}

void frmWk3260::setFrequency(double value)
{
    if(value==0.0)
    {
        on_btnFrequency_clicked();
        updateInstrument();
    }
    else
    {
        QString meter=":IMP";
        QString gpibCmd =QString("%1:FREQ %2;FREQ?").arg(meter).arg(QString::number(value));

        QString ret = clsRS::getInst().sendCommand(gpibCmd,true);

        wk3260.freq.setValue(ret.toDouble());
        UserfulFunctions::sleepMs(100);
        saveSettings();
        emit frequencySignal(wk3260.freq.toText());
    }

}

void frmWk3260::setSpeed(QString /*sp*/)
{
    dlgSpeed *dlg = new dlgSpeed();
    dlg->setWindowTitle(tr("设置测试速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        QString ret = dlg->getSpeed();
        this->wk3260.speed.setValue(ret);

        clsRS::getInst().sendCommand(wk3260.speed.toGpib(":IMP"));
        emit this->speedSignal(wk3260.speed.toText());
        saveSettings();
    }
}

void frmWk3260::setLevel(double value, QString unit)
{
    if(value ==0.0)
    {
        on_btnLevel_clicked();
        updateInstrument();
        return;

    }

    double tmpValue = value;
    if(unit=="V")
    {
        tmpValue =(tmpValue>getMaxLevelV()?getMaxLevelV():tmpValue);
        tmpValue =(tmpValue<getMinLevelV()?getMinLevelV():tmpValue);
        wk3260.level.setLevel(tmpValue,unit);

        clsRS::getInst().sendCommand(wk3260.level.toGpib(":IMP"));
        saveSettings();

    }
    else
    {
        tmpValue =(tmpValue>getMaxLevelA()?getMaxLevelA():tmpValue);
        tmpValue =(tmpValue<getMinLevelA()?getMinLevelA():tmpValue);
        wk3260.level.setLevel(tmpValue,unit);

        clsRS::getInst().sendCommand(wk3260.level.toGpib(":IMP"));
        saveSettings();
    }
    emit this->levelSignal(wk3260.level.toText());
}

void frmWk3260::setBias(double value, QString /*unit*/)
{
    wk3260.biasValue.value= value;

    clsRS::getInst().sendCommand(wk3260.biasValue.toGpib(":IMP"));
//UserfulFunctions::sleepMs(1000);
    QString ret = clsRS::getInst().sendCommand(":IMP:BIAS?",true);

    // qDebug()<<ret;

    wk3260.biasValue.value = ret.toDouble();
    emit this->biasValueSignal(wk3260.biasValue.toText(1));


    saveSettings();
}

void frmWk3260::getFrequency(double *value)
{
    *value = wk3260.freq.value;
}

void frmWk3260::getLevel(double *value, QString *unit)
{
    *value =wk3260.level.value;
    *unit = wk3260.level.unit;
}

void frmWk3260::getBias(double *value, QString *unit)
{
    *unit ="A";
    *value = wk3260.biasValue.value;
}

void frmWk3260::readSettings()
{
    readSettings(this->wk3260);
}

void frmWk3260::saveSettings()
{
    this->writeSettings(this->wk3260);
}

bool frmWk3260::turnOffBias()
{
    if(queryBiasStatus())
    {
        wk3260.biasValue.status="OFF";
        clsRS::getInst().sendCommand(wk3260.biasValue.toGpib(":IMP"));
        return queryBiasStatus();
    }
    else
    {
        return queryBiasStatus();
    }
}

bool frmWk3260::turnOnBias()
{
    if(!queryBiasStatus())
    {
        wk3260.biasValue.status="ON";
        clsRS::getInst().sendCommand(wk3260.biasValue.toGpib(":IMP"));
        return queryBiasStatus();
    }
    else
    {
        return queryBiasStatus();
    }
}

QString frmWk3260::trig()
{
    int i=0;
 RETEST:
    QString meter=":IMP";
    QString gpibCmd =QString("%1:TRIG").arg(meter);


    QString ret = clsRS::getInst().sendCommand(gpibCmd,true);
    if(ret.isEmpty() && i<5)
    {   qDebug()<< "Have entry retest procedure, and i: " <<i ;
        i++;
        goto RETEST;
    }
    return ret+",";
}

void frmWk3260::calibration()
{
    frmWK3260Calibration *dlg = new frmWK3260Calibration();
    dlg->setWindowTitle(tr("仪器校准"));
    dlg->exec();
}

void frmWk3260::updateInstrument()
{
    QString meter =":IMP";

    wk3260.item.setItem(cmbFuction1->currentText(),
                        cmbFunction2->currentText());

   // qDebug()<< cmbFunction2->currentText();
    wk3260.speed.setValue(cmbSpeed->currentText());

    wk3260.equcct.setValue(cmbEqucct->currentText());
    wk3260.biasSpeed.setValue(cmbAddBiasSpeed->currentText());
    wk3260.biasType.setValue(cmbBiasChioce->currentText());

    clsRS::getInst().sendCommand(wk3260.item.toGpib(meter));
    clsRS::getInst().sendCommand(wk3260.level.toGpib(meter));
    clsRS::getInst().sendCommand(wk3260.freq.toGpib(meter));
    clsRS::getInst().sendCommand(wk3260.speed.toGpib(meter));
    clsRS::getInst().sendCommand(wk3260.equcct.toGpib(meter));
    clsRS::getInst().sendCommand(wk3260.biasType.toGpib(meter));
    clsRS::getInst().sendCommand(wk3260.biasSpeed.toGpib(meter));
    //  qDebug()<<QTime::currentTime().toString("hh:mm:ss")<<" Bias on";
    clsRS::getInst().sendCommand(wk3260.biasValue.toGpib(meter));
    // qDebug()<<QTime::currentTime().toString("hh:mm:ss")<<" Bias off";

    emit this->frequencySignal(wk3260.freq.toText());
    emit this->speedSignal(this->wk3260.speed.toText());
    emit this->biasValueSignal(wk3260.biasValue.toText(1));
    emit this->biasStatusSignal((wk3260.biasValue.status=="ON"?true:false));
    emit this->sweepItemSignal(wk3260.item.toText(1),wk3260.item.toText(2));
    emit this->levelSignal(wk3260.level.toText());

    queryBiasStatus();

}

QString frmWk3260::getItem1()
{
    return wk3260.item.toText(1);
}

QString frmWk3260::getItem2()
{
    return wk3260.item.toText(2);
}

QString frmWk3260::getEqucct()
{
    return wk3260.equcct.toText();
}

double frmWk3260::getMaxBiasV()
{
    return 0;
}

double frmWk3260::getMinBiasV()
{
    return 0;
}

double frmWk3260::getMaxBiasA()
{
    return 125.0;
}

double frmWk3260::getMinBiasA()
{
    return 0;
}

double frmWk3260::getMaxLevelA()
{
    return 0.2;
}

double frmWk3260::getMinLevelA()
{
    return 50.0/1000000.0;
}

double frmWk3260::getMaxLevelV()
{
    return 10;
}

double frmWk3260::getMinLevelV()
{
    return 1.0/1000.0;
}

double frmWk3260::getMaxFrequency()
{
    return 3000000;
}

double frmWk3260::getMinFrequency()
{
    return 20;
}

QString frmWk3260::getSuportFunction()
{
    //"freq,BiasV,BiasA,Time"

    return "1,0,1,1";
}

void frmWk3260::on_cmbFuction1_currentIndexChanged(const QString &arg1)
{
    cmbFunction2->clear();
    if(cmbFuction1->currentText() =="Z")
    {
        cmbFunction2->addItems(QStringList()<<"θ");
    }
    else
    {
        cmbFunction2->addItems(QStringList()<<"Q"<<"D"<<"R");
    }

}

void frmWk3260::on_cmbBiasChioce_currentIndexChanged(int index)
{
    cmbAddBiasSpeed->clear();

    if(index ==0)
    {
        cmbAddBiasSpeed->addItems(QStringList()<<"OFF"<<"OnStd");
    }
    else
    {
        cmbAddBiasSpeed->addItems(QStringList()<<"OFF"<<"OnStd"<<"OnMed"<<"OnMax");
    }
}



void frmWk3260::on_btnUpdate_clicked()
{
    updateInstrument();
    writeSettings(this->wk3260);

    this->accept();
}

void frmWk3260::on_btnLevel_clicked()
{
    dlgLevelInputBox *level = new dlgLevelInputBox(this);

    level->setWindowTitle(tr("设定测试电平"));
    if(level->exec() ==QDialog::Accepted)
    {
        double dbl;
        QString unit;
        QString suffix;
        dbl=level->getValue();
        unit = level->getUnit();
        suffix = level->getSuffix();

        doubleType dt(dbl,suffix);

        double tmpValue;

        if(unit=="V")
        {
            tmpValue=((dt.Data()> getMaxLevelV()?getMaxLevelV():dt.Data()));
            tmpValue=((tmpValue< getMinLevelV()?getMinLevelV():tmpValue));
        }
        else
        {
            tmpValue=((dt.Data()> getMaxLevelA()?getMaxLevelA():dt.Data()));
            tmpValue=((tmpValue< getMinLevelA()?getMinLevelA():tmpValue));
        }

        dt.setData(tmpValue,"");
        wk3260.level.setLevel(dt.Data(),unit);

        btnLevel->setText(wk3260.level.toText());
    }
}

void frmWk3260::on_btnFrequency_clicked()
{
    NumberInput *input = new NumberInput(this);

    input->setValueAndSuffix(wk3260.freq.value,"");
    input->setWindowTitle(tr("设置测试频率"));

    if(input->exec() ==QDialog::Accepted)
    {
        wk3260.freq.setValue(input->getNumber());
        btnFrequency->setText(wk3260.freq.toText());
    }
}




void frmWk3260::readSettings(WK3260 &wk)
{
    clsSettings settings;

    if(! settings.FileExit())
        return;
      QString strNode=QString("Analysis-%1/").arg(clsRS::getInst().meterMode);

    settings.readSetting(strNode+"Equcct",wk.equcct.value);
    settings.readSetting(strNode+"LevelValue",wk.level.value);
    settings.readSetting(strNode+"LevelUnit",wk.level.unit);
    settings.readSetting(strNode+"Speed",wk.speed.value);
    settings.readSetting(strNode+"BiasSpeed",wk.biasSpeed.value);
    settings.readSetting(strNode+"BiasType",wk.biasType.value);
    settings.readSetting(strNode+"BiasValue",wk.biasValue.value);
    settings.readSetting(strNode+"BiasStatus",wk.biasValue.status);
    settings.readSetting(strNode+"Frequency",wk.freq.value);
    settings.readSetting(strNode+"Item1",wk.item.item1);
    settings.readSetting(strNode+"Item2",wk.item.item2);
    settings.readSetting(strNode+"Meter",wk.Meter);
}

void frmWk3260::writeSettings(WK3260 wk)
{
    clsSettings settings;


    QString strNode=QString("Analysis-%1/").arg(clsRS::getInst().meterMode);

    settings.writeSetting(strNode+"Equcct",wk.equcct.value);
    settings.writeSetting(strNode+"LevelValue",wk.level.value);
    settings.writeSetting(strNode+"LevelUnit",wk.level.unit);
    settings.writeSetting(strNode+"Speed",wk.speed.value);
    settings.writeSetting(strNode+"BiasSpeed",wk.biasSpeed.value);
    settings.writeSetting(strNode+"BiasType",wk.biasType.value);
    settings.writeSetting(strNode+"Frequency",wk.freq.value);
    settings.writeSetting(strNode+"Item1",wk.item.item1);
    settings.writeSetting(strNode+"Item2",wk.item.item2);
    settings.writeSetting(strNode+"Meter",wk.Meter);
    settings.writeSetting(strNode+"BiasValue",wk.biasValue.value);
    settings.writeSetting(strNode+"BiasStatus",wk.biasValue.status);

}

void frmWk3260::updateButtons()
{
    setCmbboxSelected(cmbFuction1,this->wk3260.item.item1);
    setCmbboxSelected(cmbFunction2,this->wk3260.item.item2);
    setCmbboxSelected(cmbSpeed,this->wk3260.speed.value);
    setCmbboxSelected(cmbEqucct,this->wk3260.equcct.value);
    setCmbboxSelected(cmbBiasChioce,this->wk3260.biasType.value);
    setCmbboxSelected(cmbAddBiasSpeed,this->wk3260.biasSpeed.value);

    btnFrequency->setText(this->wk3260.freq.toText());
    btnLevel->setText(this->wk3260.level.toText());
    btnBiasValue->setText(this->wk3260.biasValue.toText(1));
    btnBiasOnOFF->setText(this->wk3260.biasValue.toText(2));
}

void frmWk3260::setCmbboxSelected(QComboBox *cmb, QString tmp)
{
    for(int i =0; i< cmb->count(); i++)
    {
        if(cmb->itemText(i) ==tmp)
        {
            cmb->setCurrentIndex(i);
            return;
        }
    }
}

bool frmWk3260::queryBiasStatus()
{
    QString meter =":IMP";
    QString gpibCmd = QString("%1:BIAS-STAT?").arg(meter);

    QString ret = clsRS::getInst().sendCommand(gpibCmd,true);
    QStringList retList = ret.split(",");

    // qDebug()<<ret;
    if(retList.length()<2)
        return false;

    if(retList.at(1).toDouble()==0)
    {
        wk3260.biasType.setValue("Norm");
    }
    else
    {
        wk3260.biasType.setValue("Boost");
    }

    if(retList.at(0).toDouble()==1)
    {
        wk3260.biasValue.status="ON";
        emit this->biasStatusSignal(true);
        saveSettings();
        return true;
    }
    else
    {
        wk3260.biasValue.status="OFF";
        emit this->biasStatusSignal(false);
        saveSettings();
        return false;
    }
    saveSettings();
    return false;
}

double frmWk3260::getMaxFrequency1(QString /*value*/)
{
    return 3000000.0;
}

void frmWk3260::on_btnCancel_clicked()
{
    this->reject();
}

void frmWk3260::on_btnBiasValue_clicked()
{
    NumberInput *dlg = new NumberInput();
    dlg->setWindowTitle(tr("输入Bias值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        double tmpValue =dlg->getNumber();

        tmpValue =(tmpValue>getMaxBiasA()?getMaxBiasA():tmpValue);
        tmpValue =(tmpValue<getMinBiasA()?getMinBiasA():tmpValue);

        this->wk3260.biasValue.setValue(tmpValue,wk3260.biasValue.status);
        btnBiasValue->setText(wk3260.biasValue.toText(1));
    }

}

void frmWk3260::on_btnBiasOnOFF_clicked()
{
    if(btnBiasOnOFF->text()=="OFF")
    {
        btnBiasOnOFF->setText("ON");
        this->wk3260.biasValue.setValue(this->wk3260.biasValue.value,"ON");
    }
    else
    {
        btnBiasOnOFF->setText("OFF");
        this->wk3260.biasValue.setValue(this->wk3260.biasValue.value,"OFF");
    }
}

