#include "wk6440AnalysisMeter.h"
#include "clsSettings.h"
#include "doubleType.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "dlgSpeed.h"
#include "wk6500Range.h"
#include "dlgWk6440Function1.h"
#include "dlgWk6440Function2.h"
#include "NumberInput.h"
#include "dlgLevelInputBox.h"
#include "wk6440Calibration.h"
#include <QMessageBox>
wk6440AnalysisMeter::wk6440AnalysisMeter(WKEInstrument *parent) :
    WKEInstrument(parent)
{
    setupUi(this);
    readSettings();
    clsRS::getInst().sendCommand(":MEAS");
    clsRS::getInst().sendCommand(":MEAS:TEST");
    clsRS::getInst().sendCommand(":MEAS:TEST:AC");
}

void wk6440AnalysisMeter::setFrequency(double value)
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
        UserfulFunctions::sleepMs(20);
        gpibcmd =QString(":MEAS:FREQ?");
        QString retFreq = clsRS::getInst().sendCommand(gpibcmd,true);
        //qDebug()<<"Get Instrument frequency is: "<<retFreq;
        this->frequency = retFreq.toDouble();
        dt.setData(this->frequency,"");
        UserfulFunctions::sleepMs(20);
        emit frequencySignal(dt.formateToString(6)+"Hz");
        saveSettings();
    }
}

void wk6440AnalysisMeter::setSpeed(QString /*sp*/)
{
    on_btnSpeed_clicked();
    this->updateInstrument();
}

void wk6440AnalysisMeter::setLevel(double value, QString unit)
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
        gpibCmd.append(meter+"LEV "+QString::number(value)+unit+";");
        gpibCmd.append(meter+"LEV?"+";");
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

void wk6440AnalysisMeter::setBias(double /*value*/, QString /*unit*/)
{
    //这个在这里用不到，所以没有实现函数体。
}

void wk6440AnalysisMeter::getFrequency(double *value)
{
    *value = frequency;
}

void wk6440AnalysisMeter::getLevel(double *value, QString *unit)
{
    *unit= levelType;
    if(levelType=="V")
        *value = levelVValue;
    else
        *value = levelAValue;
}

void wk6440AnalysisMeter::getBias(double *value, QString *unit)
{
    *value=0;
    *unit="V";
}

void wk6440AnalysisMeter::readSettings()
{
    clsSettings settings;
    doubleType dt;
    QString strNode=QString("Analysis-%1/").arg(clsRS::getInst().meterSeries);

    settings.readSetting(strNode+"Item1",item1);
    item1=(item1.isEmpty()?"Z":item1);
    this->btnItem1->setText(item1);

    settings.readSetting(strNode+"Item2",item2);
    item2=(item2.isEmpty()?"A":item2);
    this->btnItem2->setText(item2);

    settings.readSetting(strNode+"Equcct",equcct);
    equcct=(equcct.isEmpty()?tr("串联"):equcct);
    this->btnEqucct->setText(equcct);

    settings.readSetting(strNode+"Speed",speed);
    speed=(speed.isEmpty()?tr("最快"):speed);
    this->btnSpeed->setText(speed);

    settings.readSetting(strNode+"Range",range);
    range=(range.isEmpty()?tr("自动"):range);
    this->btnRange->setText(range);

    settings.readSetting(strNode+"Frequecy",frequency);
    frequency=(frequency==0.0?1000:frequency);
    dt.setData(frequency,"");
    this->btnFrequency->setText(dt.formateToString(6)+"Hz");

    settings.readSetting(strNode+"LevelVValue",levelVValue);
    levelVValue=(levelVValue==0.0?1:levelVValue);
    settings.readSetting(strNode+"levelAValue",levelAValue);
    levelAValue=(levelAValue==0.0?0.1:levelAValue);
    settings.readSetting(strNode+"LevelType",levelType);
    levelType=(levelType==""?"V":levelType);


    if(levelType=="V")
    {
        dt.setData(levelVValue,"");

    }
    else
    {
        dt.setData(levelAValue,"");
    }
    this->btnLevel->setText(dt.formateToString(6)+levelType);

    settings.readSetting(strNode+"BiasType",biasType);
    biasType=(biasType.isEmpty()?tr("内置"):biasType);
    btnBiasSource->setText(biasType);

    settings.readSetting(strNode+"BiasVValue",biasVValue);
    biasVValue=(biasVValue==0?0:biasVValue);

    settings.readSetting(strNode+"BiasAValue",biasAValue);
    biasAValue=(biasAValue==0?0:biasAValue);

    settings.readSetting(strNode+"BiasONOFF",biasONOFF);
    QString strBiasONOFF=(biasONOFF?tr("开"):tr("关"));
    btnBiasStatus->setText(strBiasONOFF);
}

void wk6440AnalysisMeter::saveSettings()
{
    clsSettings settings;

    QString strNode=QString("Analysis-%1/").arg(clsRS::getInst().meterSeries);

    settings.writeSetting(strNode+"Item1",item1);
    settings.writeSetting(strNode+"Item2",item2);
    settings.writeSetting(strNode +"Equcct",equcct);
    settings.writeSetting(strNode+"Speed",speed);
    settings.writeSetting(strNode+"Range",range);
    settings.writeSetting(strNode+"Frequecy",frequency);
    settings.writeSetting(strNode+"LevelVValue",levelVValue);
    settings.writeSetting(strNode+"LevelAValue",levelAValue);
    settings.writeSetting(strNode+"LevelType",levelType);
    settings.writeSetting(strNode+"BiasType",biasType);
    settings.writeSetting(strNode+"BiasVValue",biasVValue);
    settings.writeSetting(strNode+"BiasAValue",biasAValue);
    settings.writeSetting(strNode+"BiasONOFF",biasONOFF);
}

bool wk6440AnalysisMeter::turnOffBias()
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

bool wk6440AnalysisMeter::turnOnBias()
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

QString wk6440AnalysisMeter::trig()
{
    QString strRes=clsRS::getInst().sendCommand(":MEAS:TRIG",true);
    return strRes+",";
}

void wk6440AnalysisMeter::calibration()
{
    wk6440Calibration *dlg = new wk6440Calibration(this);
    dlg->exec();

}

void wk6440AnalysisMeter::updateInstrument()
{
    QStringList gpibCmd;
    QString meter=":MEAS:";
    // QString meter2="MEAS:";

    if(equcct==tr("串联"))
        gpibCmd.append(meter+"EQU-CCT "+"SER");
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR");

    gpibCmd.append(meter+"FUNC:"+ item1 );  //item1 GPIB

    if(item1 !="Z" && item1 !="Y")
        gpibCmd.append(meter+"FUNC:"+item2 );

    if(range==tr("自动"))
        gpibCmd.append(meter+"RANGE "+"AUTO");
    else
        gpibCmd.append(meter+"RANGE "+range);

    if(speed==tr("最快"))
        gpibCmd.append(meter+"SPEED "+"MAX");
    if(speed==tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST");
    if(speed==tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED");
    else if(speed==tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW");
    else
        gpibCmd.append(meter+"SPEED "+"MAX");

    gpibCmd.append(meter+"FREQ "+QString::number(frequency));



    if(levelType=="V")
    {
        gpibCmd.append(meter+"LEV "+QString::number(levelVValue)+"V");
    }
    else
    {
        gpibCmd.append(meter+"LEV "+QString::number(levelAValue)+"A");
    }

    if(biasType==tr("内置"))
    {
        gpibCmd.append(meter+"BIAS "+"VINT");
    }
    else
    {
        gpibCmd.append(meter+"BIAS "+"VEXT");
    }
    if(!biasONOFF)
    {
        gpibCmd.append(meter+"BIAS "+"OFF");
    }
    else
    {
        gpibCmd.append(meter+"BIAS "+"ON");
    }

    for(int i=0;i<gpibCmd.length();i++)
    {
       // qDebug()<<gpibCmd.at(i);
        clsRS::getInst().sendCommand(gpibCmd.at(i),false);

        qApp->processEvents();
        UserfulFunctions::sleepMs(50);
    }

    QString xx =QString(":MEAS:FREQ?");
    QString retFreq = clsRS::getInst().sendCommand(xx,true);

    this->frequency = retFreq.toDouble();

    updateButtons();
    saveSettings();
    emit biasStatusSignal(biasONOFF);
    emit frequencySignal(btnFrequency->text());
    emit levelSignal(btnLevel->text());
    emit speedSignal(btnSpeed->text());
    emit sweepItemSignal(btnItem1->text(),btnItem2->text());
    emit biasValueSignal(this->btnBiasSource->text());

}

QString wk6440AnalysisMeter::getItem1()
{
    return this->item1;
}

QString wk6440AnalysisMeter::getItem2()
{
    return this->item2;
}

QString wk6440AnalysisMeter::getEqucct()
{
    return this->equcct;
}

double wk6440AnalysisMeter::getMaxBiasV()
{
    return 0;
}

double wk6440AnalysisMeter::getMinBiasV()
{
    return 0;
}

double wk6440AnalysisMeter::getMaxBiasA()
{
    return 0;
}

double wk6440AnalysisMeter::getMinBiasA()
{
    return 0;
}

double wk6440AnalysisMeter::getMaxLevelA()
{
    return 0.2;
}

double wk6440AnalysisMeter::getMinLevelA()
{
    return 50.0/1000000.0;
}

double wk6440AnalysisMeter::getMaxLevelV()
{
    return 10.0;
}

double wk6440AnalysisMeter::getMinLevelV()
{
    return 0.001;
}

double wk6440AnalysisMeter::getMaxFrequency()
{
    if(clsRS::getInst().instrumentModel=="6440")
        return 3E6;
    else
        return 500E3;
}

double wk6440AnalysisMeter::getMinFrequency()
{
    return 20.0;
}

QString wk6440AnalysisMeter::getSuportFunction()
{
    //"freq,BiasV,BiasA,Time"
    return "1,0,0,1";
}

bool wk6440AnalysisMeter::queryBiasStatus()
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

double wk6440AnalysisMeter::getMaxFrequency1(QString value)
{
    if(value == "6440")
        return 3E6;
    else
        return 500E3;
}

void wk6440AnalysisMeter::updateButtons()
{
    btnItem1->setText(this->item1);
    QString tmp ="ZY";
    if(tmp.contains(item1))
        btnItem2->setText("A");
    else
        btnItem2->setText(this->item2);

    btnEqucct->setText(equcct);
    btnRange->setText(range);
    btnSpeed->setText(speed);
    btnBiasSource->setText(this->biasType);
    btnBiasStatus->setText((biasONOFF?tr("开"):tr("关")));

    doubleType dt;
    dt.setData(this->frequency,"");
    btnFrequency->setText(dt.formateToString(6)+"Hz");

    if(this->levelType=="V")
    {
        dt.setData(this->levelVValue,"");
        btnLevel->setText(dt.formateToString(6)+"V");
    }
    else
    {
        dt.setData(this->levelAValue,"");
        btnLevel->setText(dt.formateToString(6)+"A");
    }
}

void wk6440AnalysisMeter::on_btnOk_clicked()
{
    btnOk->setEnabled(false);
    updateInstrument();
    saveSettings();
    this->accept();
    btnOk->setEnabled(true);
}

void wk6440AnalysisMeter::on_btnCancel_clicked()
{
    this->reject();
}

void wk6440AnalysisMeter::on_btnItem1_clicked()
{
    dlgWk6440Function1 *dlg=new dlgWk6440Function1(this);
    dlg->setWindowTitle(tr("设置测试项目1"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item1=dlg->getItem();
        this->btnItem1->setText(item1);

        if(item1=="Z"|| item1=="Y")
        {
            this->btnItem2->setText("A");
            this->item2="A";
            return ;
        }

        if(this->btnItem2->text()=="A")
        {
            this->btnItem2->setText("Q");
            item2 = "Q";
        }
    }
}

void wk6440AnalysisMeter::on_btnItem2_clicked()
{
    if(this->btnItem1->text()=="Z"|| btnItem1->text()=="Y")
    {
        this->btnItem2->setText("A");
        this->item2="A";
        return;
    }

    dlgWk6440Function2 *dlg = new dlgWk6440Function2;
    dlg->setWindowTitle(tr("设置测试项目2"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item2 = dlg->getItem();
        btnItem2->setText(item2);
    }
}

void wk6440AnalysisMeter::on_btnEqucct_clicked()
{
    QString strTmp=btnEqucct->text();

    strTmp=(strTmp==tr("串联")?tr("并联"):tr("串联"));
    btnEqucct->setText(strTmp);
    equcct=strTmp;

    if(btnItem1->text()=="B" && equcct== tr("串联"))
    {
        btnItem1->setText("X");
        this->item1="X";
    }

    if(btnItem1->text()=="X" && equcct== tr("并联"))
    {
        btnItem1->setText("B");
        this->item1="B";
    }
}

void wk6440AnalysisMeter::on_btnRange_clicked()
{
    wk6500Range *dlg=new wk6500Range;
    dlg->setWindowTitle(tr("设置档位"));

    if(dlg->exec()==QDialog::Accepted)
    {
        range=dlg->getRange();
        this->btnRange->setText(range);
    }
}

void wk6440AnalysisMeter::on_btnSpeed_clicked()
{
    dlgSpeed *dlg=new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        speed=dlg->getSpeed();
        this->btnSpeed->setText(speed);
    }
}

void wk6440AnalysisMeter::on_btnFrequency_clicked()
{
    NumberInput *dlg=new NumberInput;
    dlg->setWindowTitle(tr("设置频率"));
    doubleType dt;
    dt.setData(frequency,"");
    QString tmpUnit;
    double tmpValue;
    dt.getDataAndUnit(tmpValue,tmpUnit);
    dlg->setValueAndSuffix(tmpValue,tmpUnit);

    if(dlg->exec()==QDialog::Accepted)
    {
        frequency=dlg->getNumber();
        frequency=(frequency<20?20:frequency);
        frequency=(frequency>getMaxFrequency()?getMaxFrequency():frequency);
        dt.setData(frequency,"");
        this->btnFrequency->setText(dt.formateToString(6)+"Hz");
    }
}

void wk6440AnalysisMeter::on_btnLevel_clicked()
{
    dlgLevelInputBox *dlg=new dlgLevelInputBox(this);
    // setAVisible(false);
    dlg->setWindowTitle(tr("设置电平"));
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
        levelType=dlg->getUnit();
        double value=dlg->getValue();
        QString suffix=dlg->getSuffix();
        dt.setData(value,suffix);

        if(levelType=="V")
        {
            levelVValue=dt.Data();
            levelVValue=(levelVValue>getMaxLevelV()?getMaxLevelV():levelVValue);
            levelVValue=(levelVValue<getMinLevelV()?getMinLevelV():levelVValue);
            dt.setData(levelVValue,"");
        }
        else
        {
            levelAValue=dt.Data();
            levelAValue=(levelAValue>getMaxLevelA()?getMaxLevelA():levelAValue);
            levelAValue=(levelAValue<getMinLevelA()?getMinLevelA():levelAValue);
            dt.setData(levelAValue,"");
        }

        btnLevel->setText(dt.formateToString(6)+this->levelType);
    }

}

void wk6440AnalysisMeter::on_btnBiasSource_clicked()
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

void wk6440AnalysisMeter::on_btnBiasStatus_clicked()
{
    QString strTmp=btnBiasStatus->text();
    strTmp=(strTmp==tr("开")?tr("关"):tr("开"));
    btnBiasStatus->setText(strTmp);
    biasONOFF=(strTmp==tr("开")?true:false);
}
