#include "wk4300AnalysisMeter.h"
#include "clsSettings.h"
#include "clsRuningSettings.h"
#include "doubleType.h"
#include "wk6500Range.h"
#include "dlgFunction.h"
#include "dlgLevelInputBox.h"
#include "NumberInput.h"
#include "dlgSpeed.h"
#include "UserfulFunctions.h"
#include "wk4300Calibration.h"
wk4300AnalysisMeter::wk4300AnalysisMeter(WKEInstrument *parent) :
    WKEInstrument(parent)
{
    setupUi(this);

    readSettings();
}

void wk4300AnalysisMeter::setFrequency(double value)
{
    if(value==0)
    {
        on_btnFrequency_clicked();
        this->updateInstrument();
    }
    else
    {
        doubleType dt;
        dt.setData(value,"");
        QString gpibcmd =QString(":MEAS:FREQ %1").arg(dt.formateWithUnit("",10));
        clsRS::getInst().sendCommand(gpibcmd,false);
        gpibcmd =QString(":MEAS:FREQ?");
        QString retFreq = clsRS::getInst().sendCommand(gpibcmd,true);
        //qDebug()<<"Get Instrument frequency is: "<<retFreq;
        this->frequency = retFreq.toDouble();
        dt.setData(this->frequency,"");
        emit frequencySignal(dt.formateToString()+"Hz");
        saveSettings();
    }

}

void wk4300AnalysisMeter::setSpeed(QString /*sp*/)
{
    on_btnSpeed_clicked();
    this->updateInstrument();
}

void wk4300AnalysisMeter::setLevel(double value, QString unit)
{
    if(value==0)
    {
        on_btnLevel_clicked();
        this->updateInstrument();
    }
    else
    {
        doubleType dt;
        QString gpibCmd="";
        QString meter=":MEAS:";
        gpibCmd.append(meter+"LEVEL "+QString::number(value)+unit+";");
        gpibCmd.append(meter+"LEVEL?"+";");
        QString retRes= clsRS::getInst().sendCommand(gpibCmd,true);

        levelType="V";
        levelVValue = retRes.toDouble();
        dt.setData(levelVValue,"");
        emit levelSignal(dt.formateToString(6)+"V");
        saveSettings();
    }
}

void wk4300AnalysisMeter::setBias(double /*value*/, QString /*unit*/)
{

}

void wk4300AnalysisMeter::getFrequency(double *value)
{
    *value = frequency;
}

void wk4300AnalysisMeter::getLevel(double *value, QString *unit)
{
    *unit="V";
    *value = this->levelVValue;
}

void wk4300AnalysisMeter::getBias(double *value, QString *unit)
{
    *value=0;
    *unit="V";
}

void wk4300AnalysisMeter::readSettings()
{
    clsSettings settings;
    doubleType dt;
    QString strNode=QString("WK4300");

    settings.readSetting(strNode+"Item1",item1);
    item1 = (item1.isEmpty()? "Z":item1);
    this->btnItem1->setText(item1);

    settings.readSetting(strNode+"Item2",item2);
    item2 = (item2.isEmpty()? "A":item2);
    this->btnItem2->setText(item2);

    settings.readSetting(strNode +"Equcct",equcct);
    equcct = (equcct.isEmpty()? tr("串联"):equcct);
    this->btnEqucct->setText(equcct);

    settings.readSetting(strNode+"Range",range);
    range=(range.isEmpty()?tr("自动"):range);
    this->btnRange->setText(range);

    settings.readSetting(strNode+"Frequency",frequency);
    frequency=(frequency==0.0 ? 10000:frequency);
    dt.setData(frequency,"");
    this->btnFrequency->setText(dt.formateToString(6)+"Hz");

    settings.readSetting(strNode+"LevelAValue",levelAValue);
    levelAValue=(levelAValue==0.0 ? 0.1:levelAValue);
    settings.readSetting(strNode+"LevelVValue",levelVValue);
    levelVValue=(levelVValue==0.0 ? 1:levelVValue);
    settings.readSetting(strNode+"LevelType",levelType);
    levelType=(levelType=="" ? "V":levelType);

    if(levelType=="V")
    {
        dt.setData(levelVValue,"");
    }
    else
    {
        dt.setData(levelAValue,"");
    }
    btnLevel->setText(dt.formateToString(6)+levelType);

    settings.readSetting(strNode+"BiasONOFF",biasONOFF);
    QString strBiasStatus;
    strBiasStatus=(biasONOFF?tr("开"):tr("关"));
    btnBiasStatus->setText(strBiasStatus);

    settings.readSetting(strNode+"BiasType",biasType);
    biasType=(biasType.isEmpty()?tr("内置"):biasType);
    btnBiasSource->setText(biasType);

    settings.readSetting(strNode+"BiasAValue",biasAValue);
    biasAValue =(biasAValue==0?0:biasAValue);

    settings.readSetting(strNode+"BiasVValue",biasVValue);
    biasVValue=(biasVValue==0?0:biasVValue);



    settings.readSetting(strNode+"Speed",Speed);
    Speed=(Speed.isEmpty()? tr("最快"):Speed);
    btnSpeed->setText(Speed);

}

void wk4300AnalysisMeter::saveSettings()
{
    clsSettings settings;
    QString strNode=QString("WK4300");
    settings.writeSetting(strNode+"Item1",item1);
    settings.writeSetting(strNode+"Item2",item2);
    settings.writeSetting(strNode +"Equcct",equcct);
    settings.writeSetting(strNode+"Range",range);
    settings.writeSetting(strNode+"Frequency",frequency);
    settings.writeSetting(strNode+"LevelAValue",levelAValue);
    settings.writeSetting(strNode+"LevelVValue",levelVValue);
    settings.writeSetting(strNode+"LevelType",levelType);
    settings.writeSetting(strNode+"BiasONOFF",biasONOFF);
    settings.writeSetting(strNode+"BiasType",biasType);
    settings.writeSetting(strNode+"BiasAValue",biasAValue);
    settings.writeSetting(strNode+"BiasVValue",biasVValue);
    settings.writeSetting(strNode+"Speed",Speed);
}

bool wk4300AnalysisMeter::turnOffBias()
{
    if(queryBiasStatus())
    {
        QString meter=":MEAS:";
        QString gpibCmd;
        gpibCmd.append(meter+"BIAS "+"OFF");
        clsRS::getInst().sendCommand(gpibCmd,false);

        return queryBiasStatus();
    }
    else
    {
        return false;
    }

}

bool wk4300AnalysisMeter::turnOnBias()
{
    if(!queryBiasStatus())
    {
        QString meter=":MEAS:";
        QString gpibCmd;
        gpibCmd.append(meter+"BIAS "+"ON");
        clsRS::getInst().sendCommand(gpibCmd,false);
        return queryBiasStatus();
    }
    else
    {
        return true;
    }
}

QString wk4300AnalysisMeter::trig()
{
    QString strRes=clsRS::getInst().sendCommand(":MEAS:TRIG",true);
    return strRes+",";
}

void wk4300AnalysisMeter::calibration()
{
    wk4300Calibration *dlg=new wk4300Calibration();
    dlg->exec();
}

void wk4300AnalysisMeter::updateInstrument()
{
    QStringList gpibCmd;
    QString meter=":MEAS:";

    gpibCmd.append(meter+"NUM-OF-TESTS 1");

    QString item11=( item1==QString("θ")?"A":item1);
    gpibCmd.append(meter+"FUNC1 "+ item11 );  //item1 GPIB

    QString item22=( item2==QString("θ")?"A":item2);
    gpibCmd.append(meter+"FUNC2 "+item22 );  //Item2 GPIB

    if(equcct==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER" );
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR" );

    if(range ==tr("自动"))                        //档位
        gpibCmd.append(meter+"RANGE "+"AUTO" );
    else
        gpibCmd.append(meter+"RANGE "+range );

    if(Speed ==tr("最快"))                        //速度
        gpibCmd.append(meter+"SPEED "+"MAX" );
    else if(Speed ==tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST" );
    else if(Speed ==tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED" );
    else if(Speed ==tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW" );
    else
        gpibCmd.append(meter+"SPEED "+"MAX" );

    doubleType dt;
    dt.setData(frequency,"");

    gpibCmd.append(meter+"FREQ "+dt.formateWithUnit("",10) );   //频率
    if(levelType=="V")                                              //电平
        gpibCmd.append(meter+"LEV "+QString::number(levelVValue) );
    else
        gpibCmd.append(meter+"LEV "+QString::number(levelAValue) );

    if(biasType==tr("内置"))
        gpibCmd.append(meter+"BIAS "+"VINT" );

    else
        gpibCmd.append(meter+"BIAS "+"VEXT" );


    if(biasONOFF==true)
        gpibCmd.append(meter+"BIAS "+"ON" );
    else
        gpibCmd.append(meter+"BIAS "+"OFF" );

    // qDebug()<<"GPIB command " <<gpibCmd;

    for(int i=0;i<gpibCmd.length();i++)
    {
        // qDebug()<<gpibCmd.at(i);
        clsRS::getInst().sendCommand(gpibCmd.at(i),false);

        qApp->processEvents();
        UserfulFunctions::sleepMs(50);
    }

    updateButtons();
    saveSettings();
    emit biasStatusSignal(biasONOFF);
    emit frequencySignal(btnFrequency->text());
    emit levelSignal(btnLevel->text());
    emit speedSignal(btnSpeed->text());
    emit sweepItemSignal(btnItem1->text(),btnItem2->text());
    emit biasValueSignal(btnBiasSource->text());



}

QString wk4300AnalysisMeter::getItem1()
{
    return this->item1;
}

QString wk4300AnalysisMeter::getItem2()
{
    return this->item2;
}

double wk4300AnalysisMeter::getMaxBiasV()
{
    return 0;
}

double wk4300AnalysisMeter::getMinBiasV()
{
    return 0;
}

double wk4300AnalysisMeter::getMaxBiasA()
{
    return 0;
}

double wk4300AnalysisMeter::getMinBiasA()
{
    return 0;
}

double wk4300AnalysisMeter::getMaxLevelA()
{
    return 0;
}

double wk4300AnalysisMeter::getMinLevelA()
{
    return 0;
}

double wk4300AnalysisMeter::getMaxLevelV()
{
    return 2;
}

double wk4300AnalysisMeter::getMinLevelV()
{
    return 0.01;
}

double wk4300AnalysisMeter::getMaxFrequency()
{
    return getMaxFrequency1(clsRS::getInst().instrumentModel);
}

double wk4300AnalysisMeter::getMinFrequency()
{
    return 20;
}

QString wk4300AnalysisMeter::getSuportFunction()
{
    //"freq,BiasV,BiasA,Time"
    return "1,0,0,1";
}


void wk4300AnalysisMeter::on_btnOk_clicked()
{
    btnOk->setEnabled(false);
    updateInstrument();
    saveSettings();
    btnOk->setEnabled(true);

    this->accept();

}

void wk4300AnalysisMeter::on_btnBiasStatus_clicked()
{
    QString strTmp = btnBiasStatus->text();

    strTmp = (strTmp==tr("关")?tr("开"):tr("关"));

    btnBiasStatus->setText(strTmp);
    biasONOFF =(strTmp==tr("关")? false:true);
}

void wk4300AnalysisMeter::on_btnEqucct_clicked()
{
    QString strTmp = btnEqucct->text();

    strTmp = (strTmp==tr("串联")?tr("并联"):tr("串联"));
    btnEqucct->setText(strTmp);
    equcct = strTmp;
}

void wk4300AnalysisMeter::on_btnRange_clicked()
{
    wk6500Range * dialog= new wk6500Range;
    dialog->setWindowTitle(tr("设置4300的测试档位"));

    if(dialog->exec()==QDialog::Accepted)
    {
        range = dialog->getRange();
        this->btnRange->setText(range);
    }
}

void wk4300AnalysisMeter::on_btnItem1_clicked()
{
    dlgFunction *dlg = new dlgFunction;
    dlg->setWindowTitle(tr("设置4300的测试项目1"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item1 = dlg->getItem();

        this->btnItem1->setText(item1);
    }
}

void wk4300AnalysisMeter::on_btnItem2_clicked()
{
    dlgFunction *dlg = new dlgFunction;
    dlg->setWindowTitle(tr("设置4300的测试项目2"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item2 = dlg->getItem();

        this->btnItem2->setText(item2);
    }
}

void wk4300AnalysisMeter::on_btnFrequency_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设定测试频率"));
    doubleType dt;
    dt.setData(frequency,"");
    QString tmpUnit;
    double tmpValue;
    dt.getDataAndUnit(tmpValue,tmpUnit);
    dlg->setValueAndSuffix(tmpValue,tmpUnit);
    if(dlg->exec()==QDialog::Accepted)
    {
        frequency = dlg->getNumber();
        frequency =(frequency<20? 20:frequency);
        dt.setData(frequency,"");

        btnFrequency->setText(dt.formateToString(6)+"Hz");
    }

}

void wk4300AnalysisMeter::on_btnLevel_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox;
    dlg->setAVisible(false);
    dlg->setWindowTitle(tr("设置测试电平"));

    doubleType dt;

    if(levelType=="V")
        dt.setData(levelVValue,"");
    else
        dt.setData(levelAValue,"");

    QString tmpUnit;
    double tmpValue;
    dt.getDataAndUnit(tmpValue,tmpUnit);
    dlg->setValueAndSuffix(tmpValue,tmpUnit);

    if(dlg->exec()==QDialog::Accepted)
    {
        QString unit = dlg->getUnit();
        QString suffix= dlg->getSuffix();
        double value = dlg->getValue();
        dt.setData(value,suffix);

        if(unit =="V")
        {
            levelVValue = dt.Data();
            levelVValue=(levelVValue>2?2:levelVValue);
            levelVValue=(levelVValue<0.01?0.01:levelVValue);
            dt.setData(levelVValue,"");
        }
        else
        {
            levelAValue = dt.Data();
            levelAValue = (levelAValue>0.02? 0.02: levelAValue);
            levelAValue=(levelAValue<0.0002?0.0002:levelAValue);
            dt.setData(levelAValue,"");
        }

        levelType = unit;
        btnLevel->setText(dt.formateToString(6)+unit);
    }
}

void wk4300AnalysisMeter::on_btnSpeed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed;
    dlg->setWindowTitle(tr("设置测试速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        Speed = dlg->getSpeed();
        btnSpeed->setText(Speed);
    }
}

void wk4300AnalysisMeter::on_btnBiasSource_clicked()
{
    if(btnBiasSource->text()==tr("内置"))
    {
        btnBiasSource->setText(tr("外置"));
        biasType=tr("外置");
    }
    else
    {
        btnBiasSource->setText(tr("内置"));
        biasType=tr("内置");
    }
}

bool wk4300AnalysisMeter::queryBiasStatus()
{
    QString meter=":MEAS:";
    QString gpibCmd="";
    gpibCmd.append(meter+"BIAS-STAT?");
    QString strRes=clsRS::getInst().sendCommand(gpibCmd,true);
    bool ok;
    int res=strRes.toDouble(&ok);

    QStringList lstRes = strRes.split(",");
    if(lstRes.length()==2)
    {
        int i = lstRes.at(0).toInt();

        biasONOFF =(i==0?false:true);

        int j= lstRes.at(1).toInt();
        biasType =(j==0?tr("内置"):tr("外置"));

        saveSettings();

        emit biasStatusSignal(biasONOFF);
        emit biasValueSignal(biasType);
        return biasONOFF;
    }
    else
    {
        QMessageBox::warning(this,tr("WKE FactoryView 2014"),
                             tr("不能正确的查询Bias的状态，请检查连接。"),
                             QMessageBox::Ok);
        biasONOFF=false;
        emit biasStatusSignal(biasONOFF);
        saveSettings();
    }

    if(ok)
    {
        biasONOFF=(res==0?false:true);
        saveSettings();
        emit biasStatusSignal(biasONOFF);
        return biasONOFF;
    }


    return false;
}

double wk4300AnalysisMeter::getMaxFrequency1(QString value)
{
    if(value.contains("41100"))
    {
        return 1000000;
    }
    else if(value.contains("4150"))
    {
        return 500000;
    }
    else if(value.contains("4120"))
    {
        return 200000;
    }
    else if(value.contains("4110"))
    {
        return 100000;
    }
    else if(value.contains("43100"))
    {
        return 1000000;
    }
    else if(value.contains("4350"))
    {
        return 500000;
    }
    else if(value.contains("4320"))
    {
        return 200000;
    }
    else if(value.contains("4310"))
    {
        return 100000;
    }

    return 1000000;
}

void wk4300AnalysisMeter::updateButtons()
{
    doubleType dt;
    this->btnItem1->setText(item1);
    this->btnItem2->setText(item2);
    this->btnEqucct->setText(equcct);
    this->btnRange->setText(range);
    dt.setData(frequency,"");
    this->btnFrequency->setText(dt.formateToString(6)+"Hz");
    if(levelType=="V")
    {
        dt.setData(levelVValue,"");
    }
    else
    {
        dt.setData(levelAValue,"");
    }
    btnLevel->setText(dt.formateToString(6)+levelType);

    QString strBiasStatus;
    strBiasStatus=(biasONOFF?tr("开"):tr("关"));
    btnBiasStatus->setText(strBiasStatus);
    btnBiasSource->setText(biasType);
    btnSpeed->setText(Speed);
}





void wk4300AnalysisMeter::on_btnCacel_clicked()
{
    this->reject();
}
