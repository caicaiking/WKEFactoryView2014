#include "wk6500AnalysisMeter.h"
#include "clsSettings.h"
#include "clsRuningSettings.h"
#include "doubleType.h"
#include "wk6500Range.h"
#include "dlgFunction.h"
#include "dlgLevelInputBox.h"
#include "NumberInput.h"
#include "dlgSpeed.h"
#include "wk6500Calibration.h"
#include <QMessageBox>
wk6500AnalysisMeter::wk6500AnalysisMeter(WKEInstrument *parent) :
    WKEInstrument(parent)
{
    setupUi(this);

    readSettings();

}

void wk6500AnalysisMeter::setFrequency(double value)
{
    if(value==0)
    {
        on_btnFrequency_clicked();
        this->updateInstrument();
    }
    else
    {
        doubleType dt;
        dt.setData(value);
        QString gpibcmd =QString(":METER:FREQ %1").arg(QString::number(value,'f',6));
        // qDebug()<< "Input value: "<< gpibcmd;
        clsRS::getInst().sendCommand(gpibcmd,false);

        gpibcmd =QString(":METER:FREQ?");

        QString retFreq = clsRS::getInst().sendCommand(gpibcmd,true);

        // qDebug()<<"ret freq: "<< retFreq;

        this->frequency = retFreq.toDouble();
        dt.setData(this->frequency,"");
        emit frequencySignal(dt.formateToString()+"Hz");
        saveSettings();
    }

}

void wk6500AnalysisMeter::setSpeed(QString /*sp*/)
{
    on_btnSpeed_clicked();
    this->updateInstrument();
}

void wk6500AnalysisMeter::setLevel(double value, QString unit)
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
        QString meter=":METER:";
        gpibCmd.append(meter+"LEVEL "+QString::number(value)+unit+";");
        gpibCmd.append(meter+"LEVEL?"+";");
        QString retRes= clsRS::getInst().sendCommand(gpibCmd,true);

        if(unit=="V")
        {
            levelType="V";
            levelVValue = retRes.toDouble();
            dt.setData(levelVValue,"");
            emit levelSignal(dt.formateToString(6)+"V");
        }
        else
        {
            levelType="A";
            levelAValue=retRes.toDouble();
            dt.setData(levelAValue,"");
            emit levelSignal(dt.formateToString(6)+"A");
        }

        saveSettings();
    }
}

void wk6500AnalysisMeter::setBias(double value, QString unit)
{
    QString gpibCmd="";
    QString meter=":METER:";
    if(unit=="V")                          //Bias
    {
        biasType=tr("电压");
        QString retBiasType=clsRS::getInst().sendCommand(meter+"BIAS-TYPE?;",true);

        if(retBiasType.toDouble()!=1)
            clsRS::getInst().sendCommand(meter+"BIAS-TYPE "+"VOL"+";",false);

        biasVValue=value;
        gpibCmd.append(meter+"BIAS "+QString::number(biasVValue)+";");
    }
    else
    {
        biasType=tr("电流");
        QString retBiasType=clsRS::getInst().sendCommand(meter+"BIAS-TYPE?;",true);

        if(retBiasType.toDouble()!=0)
            clsRS::getInst().sendCommand(meter+"BIAS-TYPE "+"CUR"+";",false);

        biasAValue = value;
        gpibCmd.append(meter+"BIAS "+QString::number(biasAValue)+";");
    }

    gpibCmd.append(meter+"BIAS?"+";");
    QString retRes= clsRS::getInst().sendCommand(gpibCmd,true);

    if(unit=="V")
    {
        biasVValue=retRes.toDouble();
    }
    else
    {
        biasAValue = retRes.toDouble();
        //qDebug()<< biasAValue;
    }
    saveSettings();

    doubleType dt;
    dt.setData(value,"");

    emit biasValueSignal(dt.formateToString(6)+unit);

}

void wk6500AnalysisMeter::getFrequency(double *value)
{
    *value = this->frequency;
}

void wk6500AnalysisMeter::getLevel(double *value, QString *unit)
{
    *unit= levelType;
    if(levelType=="V")
        *value = levelVValue;
    else
        *value = levelAValue;
}

void wk6500AnalysisMeter::getBias(double *value, QString *unit)
{
    if( biasType==tr("电压"))
    {
        *unit="V";
        *value = biasVValue;
    }
    else
    {
        *unit="A";
        *value=biasAValue;
    }
}

void wk6500AnalysisMeter::updateButtons()
{
    btnItem1->setText(item1);
    btnItem2->setText(item2);

    btnEqucct->setText(equcct);
    btnSpeed->setText(Speed);
    btnRange->setText(range);

    doubleType dt;
    dt.setData(frequency,"");
    btnFrequency->setText(dt.formateToString(7)+"Hz");

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
    btnBiasType->setText(biasType);

    if(biasType==tr("电压"))
    {
        dt.setData(biasVValue,"");
        btnBiasLevel->setText(dt.formateToString(6)+"V");
    }
    else
    {
        dt.setData(biasAValue,"");
        btnBiasLevel->setText(dt.formateToString(6)+"A");
    }
}

void wk6500AnalysisMeter::readSettings()
{
    clsSettings settings;
    doubleType dt;
    QString strNode=QString("Analysis-%1/").arg(clsRS::getInst().meterSeries);

    settings.readSetting(strNode+"Item1",item1);
    item1 = (item1.isEmpty()? "Z":item1);
    this->btnItem1->setText(item1);

    settings.readSetting(strNode+"Item2",item2);
    item2 = (item2.isEmpty()? QString("θ"):item2);
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
    biasType=(biasType.isEmpty()?tr("电压"):biasType);
    btnBiasType->setText(biasType);

    settings.readSetting(strNode+"BiasAValue",biasAValue);
    biasAValue =(biasAValue==0?0:biasAValue);

    settings.readSetting(strNode+"BiasVValue",biasVValue);
    biasVValue=(biasVValue==0?0:biasVValue);

    if(biasType==tr("电压"))
    {
        dt.setData(biasVValue,"");
        btnBiasLevel->setText(dt.formateToString(5)+"V");
    }
    else
    {
        dt.setData(biasAValue,"");
        btnBiasLevel->setText(dt.formateToString(5)+"A");
    }

    settings.readSetting(strNode+"Speed",Speed);
    Speed=(Speed.isEmpty()? tr("最快"):Speed);
    btnSpeed->setText(Speed);

}

void wk6500AnalysisMeter::saveSettings()
{
    clsSettings settings;
    QString strNode=QString("Analysis-%1/").arg(clsRS::getInst().meterSeries);
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

bool wk6500AnalysisMeter::turnOffBias()
{
    if(queryBiasStatus())
    {
        QString meter=":METER:";
        QString gpibCmd;
        gpibCmd.append(meter+"BIAS-STAT "+"OFF");
        clsRS::getInst().sendCommand(gpibCmd,false);

        return queryBiasStatus();
    }
    else
    {
        return false;
    }

}

bool wk6500AnalysisMeter::turnOnBias()
{
    if(!queryBiasStatus())
    {
        QString meter=":METER:";
        QString gpibCmd;
        gpibCmd.append(meter+"BIAS-STAT "+"ON");
        clsRS::getInst().sendCommand(gpibCmd,false);
        return queryBiasStatus();
    }
    else
    {
        return true;
    }
}

QString wk6500AnalysisMeter::trig()
{
    QString strRes=clsRS::getInst().sendCommand(":METER:TRIG",true);
    return strRes+",";
}

void wk6500AnalysisMeter::calibration()
{
    wk6500Calibration *dlg = new wk6500Calibration(getMaxFrequency());

    //dlg->setMaxFreq(getMaxFrequency());
    dlg->exec();
}

void wk6500AnalysisMeter::updateInstrument()
{
    QString gpibCmd;
    QString meter=":METER:";

    gpibCmd.append(meter+"FUNC:1 "+item1+";");  //item1 GPIB
    gpibCmd.append(meter+"FUNC:2 "+item2+";");  //Item2 GPIB

    if(equcct==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER"+";");
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR"+";");

    if(range ==tr("自动"))                        //档位
        gpibCmd.append(meter+"RANGE "+"AUTO"+";");
    else
        gpibCmd.append(meter+"RANGE "+range+";");

    if(Speed ==tr("最快"))                        //速度
        gpibCmd.append(meter+"SPEED "+"MAX"+";");
    else if(Speed ==tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST"+";");
    else if(Speed ==tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED"+";");
    else if(Speed ==tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW"+";");
    else
        gpibCmd.append(meter+"SPEED "+"MAX"+";");

    gpibCmd.append(meter+"FREQ "+QString::number(frequency,'f',6)+";");   //频率
    if(levelType=="V")                                              //电平
        gpibCmd.append(meter+"LEVEL "+QString::number(levelVValue)+levelType+";");
    else
        gpibCmd.append(meter+"LEVEL "+QString::number(levelAValue)+levelType+";");

    if(biasType==tr("电压"))                          //Bias
    {
        gpibCmd.append(meter+"BIAS-TYPE "+"VOL"+";");
        gpibCmd.append(meter+"BIAS "+QString::number(biasVValue)+";");
    }
    else
    {
        gpibCmd.append(meter+"BIAS-TYPE "+"CUR"+";");
        gpibCmd.append(meter+"BIAS "+QString::number(biasAValue)+";");
    }

    if(biasONOFF==true)
        gpibCmd.append(meter+"BIAS-STAT "+"ON"+";");
    else
        gpibCmd.append(meter+"BIAS-STAT "+"OFF"+";");

    // qDebug()<<"GPIB command " <<gpibCmd;

    gpibCmd = gpibCmd.replace(QString("θ"),QString("ANGLE"));

     //qDebug()<< gpibCmd;
    clsRS::getInst().sendCommand(gpibCmd);

    updateButtons();
    saveSettings();
    emit biasStatusSignal(biasONOFF);
    emit frequencySignal(btnFrequency->text());
    emit levelSignal(btnLevel->text());
    emit speedSignal(btnSpeed->text());
    emit sweepItemSignal(btnItem1->text(),btnItem2->text());
    emit biasValueSignal(btnBiasLevel->text());

}

QString wk6500AnalysisMeter::getItem1()
{
    return item1;
}

QString wk6500AnalysisMeter::getItem2()
{
    return item2;
}

QString wk6500AnalysisMeter::getEqucct()
{
    //  qDebug()<< "get Equ-cct:"<< equcct;
    return this->equcct;
}

double wk6500AnalysisMeter::getMaxBiasV()
{
    return 40;
}

double wk6500AnalysisMeter::getMinBiasV()
{
    return -40;
}

double wk6500AnalysisMeter::getMaxBiasA()
{
    static double maxC =0;

    if(maxC==0)
    {
        clsRS::getInst().sendCommand("*SYSBIAS \'BIAS EXT\'",false);
        if(clsRS::getInst().sendCommand("*SYSBIAS?",true)=="BIAS EXT")
        {
            maxC=40.0;
            return maxC;
        }

        clsRS::getInst().sendCommand("*SYSBIAS \'BIAS INT\'",false);
        if(clsRS::getInst().sendCommand("*SYSBIAS?",true)=="BIAS INT")
        {
            maxC=0.1;
            return maxC;
        }
    }
    return maxC;
}

double wk6500AnalysisMeter::getMinBiasA()
{
    return 0;
}

double wk6500AnalysisMeter::getMaxLevelA()
{
    return 0.02;
}

double wk6500AnalysisMeter::getMinLevelA()
{
    return 0.0002;
}

double wk6500AnalysisMeter::getMaxLevelV()
{
    return 1;
}

double wk6500AnalysisMeter::getMinLevelV()
{
    return 0.01;
}

double wk6500AnalysisMeter::getMaxFrequency()
{
    return getMaxFrequency1(clsRS::getInst().instrumentModel);
}

double wk6500AnalysisMeter::getMinFrequency()
{
    return 20;
}

QString wk6500AnalysisMeter::getSuportFunction()
{
    //"freq,BiasV,BiasA,Time"

    return "1,1,1,1";
}

void wk6500AnalysisMeter::on_btnCancel_clicked()
{
    this->reject();
}

void wk6500AnalysisMeter::on_btnOk_clicked()
{
    updateInstrument();
    saveSettings();
    this->accept();
}

void wk6500AnalysisMeter::on_btnBiasType_clicked()
{
    QString strTmp = btnBiasType->text();

    strTmp = (strTmp==tr("电压")?tr("电流"):tr("电压"));

    btnBiasType->setText(strTmp);
    biasType = strTmp;

    doubleType dt;
    if(biasType==tr("电压"))
    {
        dt.setData(biasVValue,"");
        btnBiasLevel->setText(dt.formateToString(6)+"V");
    }
    else
    {
        dt.setData(biasAValue,"");
        btnBiasLevel->setText(dt.formateToString(6)+"A");
    }

}

void wk6500AnalysisMeter::on_btnBiasStatus_clicked()
{
    QString strTmp = btnBiasStatus->text();

    strTmp = (strTmp==tr("关")?tr("开"):tr("关"));

    btnBiasStatus->setText(strTmp);
    biasONOFF =(strTmp==tr("关")? false:true);
}

void wk6500AnalysisMeter::on_btnEqucct_clicked()
{
    QString strTmp = btnEqucct->text();

    strTmp = (strTmp==tr("串联")?tr("并联"):tr("串联"));
    btnEqucct->setText(strTmp);
    equcct = strTmp;
    //qDebug()<< "Equ-cct:"<< equcct;

}

void wk6500AnalysisMeter::on_btnRange_clicked()
{
    wk6500Range * dialog= new wk6500Range;
    dialog->setWindowTitle(tr("设置6500的测试档位"));

    if(dialog->exec()==QDialog::Accepted)
    {
        range = dialog->getRange();
        this->btnRange->setText(range);
    }
}

void wk6500AnalysisMeter::on_btnItem1_clicked()
{
    dlgFunction *dlg = new dlgFunction;
    dlg->setWindowTitle(tr("设置6500的测试项目1"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item1 = dlg->getItem();

        this->btnItem1->setText(item1);
    }
}

void wk6500AnalysisMeter::on_btnItem2_clicked()
{
    dlgFunction *dlg = new dlgFunction;
    dlg->setWindowTitle(tr("设置6500的测试项目2"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item2 = dlg->getItem();

        this->btnItem2->setText(item2);
    }
}

void wk6500AnalysisMeter::on_btnFrequency_clicked()
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
        double maxFreq =getMaxFrequency1(clsRS::getInst().instrumentModel);
        frequency =(frequency>maxFreq?maxFreq:frequency);
        dt.setData(frequency,"");

        btnFrequency->setText(dt.formateToString(6)+"Hz");
    }

}

void wk6500AnalysisMeter::on_btnLevel_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox;
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
            levelVValue=(levelVValue>1?1:levelVValue);
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

void wk6500AnalysisMeter::on_btnBiasLevel_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设置偏置值"));

    doubleType dt;

    if(biasType==tr("电压"))
        dt.setData(biasVValue,"");
    else
        dt.setData(biasAValue,"");

    QString tmpUnit;
    double tmpValue;
    dt.getDataAndUnit(tmpValue,tmpUnit);

    if(dlg->exec()==QDialog::Accepted)
    {
        double value = dlg->getNumber();
        if(biasType==tr("电压"))
        {
            biasVValue =value;
            biasVValue=(biasVValue<-40?-40:biasVValue);
            biasVValue=(biasVValue>40?40:biasVValue);
            dt.setData(biasVValue,"");
            btnBiasLevel->setText(dt.formateToString(6)+"V");
        }
        else
        {
            double maxC = getMaxBiasA();
            biasAValue =value;
            biasAValue=(biasAValue<0?0:biasAValue);
            biasAValue=(biasAValue>maxC?maxC:biasAValue);
            dt.setData(biasAValue,"");
            btnBiasLevel->setText(dt.formateToString(6)+"A");
        }
    }

}

void wk6500AnalysisMeter::on_btnSpeed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed;
    dlg->setWindowTitle(tr("设置测试速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        Speed = dlg->getSpeed();
        btnSpeed->setText(Speed);
    }
}

bool wk6500AnalysisMeter::queryBiasStatus()
{
    QString meter=":METER:";
    QString gpibCmd="";
    gpibCmd.append(meter+"BIAS-STAT?");
    QString strRes=clsRS::getInst().sendCommand(gpibCmd,true);

    // qDebug()<< "bias status"<< strRes;

    int res=strRes.toDouble();


    biasONOFF=(res==0?false:true);
    saveSettings();
    emit biasStatusSignal(biasONOFF);
    return biasONOFF;

}

double wk6500AnalysisMeter::getMaxFrequency1(QString value)
{
    if(value.contains("6505"))
    {
        return 5000000;
    }
    else if(value.contains("6510"))
    {
        return 10000000;
    }
    else if(value.contains("6515"))
    {
        return 15000000;
    }
    else if(value.contains("6520"))
    {
        return 20000000;
    }
    else if(value.contains("6530"))
    {
        return 30000000;
    }
    else if(value.contains("6550"))
    {
        return 50000000;
    }
    else if(value.contains("65120"))
    {
        return 120000000;
    }
    return 120000000;
}
