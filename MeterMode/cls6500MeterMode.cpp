#include "cls6500MeterMode.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "NumberInput.h"
#include "clsRuningSettings.h"
#include "dlgLevelInputBox.h"
#include "dlgFunction.h"
#include "clsMeterUnit.h"
#include "frmSetLimit.h"
#include "wk6500Calibration.h"
cls6500MeterMode::cls6500MeterMode(WKEMeterMode *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);

    connect(this->lblItem1Limit,SIGNAL(Clicked()),this,SLOT(lblLmItem1()));
    connect(this->lblItem2Limit,SIGNAL(Clicked()),this,SLOT(lblLmItem2()));
    connect(this->lblFreqLimit,SIGNAL(Clicked()),this,SLOT(lblLmFreq()));
    connect(this->lblRLimit,SIGNAL(Clicked()),this,SLOT(lblLmR()));
    connect(this->lblCLimit,SIGNAL(Clicked()),this,SLOT(lblLmC()));
    connect(this->lblLLimit,SIGNAL(Clicked()),this,SLOT(lblLmL()));

    intSelectMode =0; //模式的是LCR测试模式
    range=tr("自动");
    speed=tr("最快");
    equcct=tr("串联");
    levelType="V";
    levelAValue=0.01;
    levelVValue=1;
    frequency=10000;

    biasVValue=0;
    biasAValue=0;
    biasType=tr("电压");
    strBiasStatus=tr("关");
    item1="C";
    item2="D";
    item1Suffix="";
    item2Suffix="";

    //Res Mode default
    freqStart=1000;
    freqStop = 1000000;
    resEqucct = tr("串联");
    resSpeed = tr("最快");
    resDepth = 16;
    blFreq = true;
    blR = false;
    blC = false;
    blL = false;
    freqSuffix="";
    rSuffix ="";
    cSuffix ="";
    lSuffix ="";

    updateButtons();
}

bool cls6500MeterMode::detectDut()
{

    isStop= true;
    bool isEmpty=false;

    QString item;


    int index = clsRS::getInst().sendCommand(":METER:FUNC:1?",true).toInt();

    switch (index) {
    case 0:
        item="L";
        break;
    case 1:
        item="C";
        break;
    case 2:
        item="R";
        break;
    case 3:
        item="Z";

        break;
    case 4:
        item="Y";
        break;
    case 5:
        item="X";
        break;
    case 6:
        item="G";
        break;
    case 7:
        item ="B";
        break;
    case 8:
        item="Q";
        break;
    case 9:
        item ="D";
        break;
    case 10:
        item =QString("θ");
        break;

    default:
        break;
    }

    if(item!="C")
        clsRS::getInst().sendCommand(QString(":METER:FUNC:1 C"),false);


    while(isStop)
    {
        QString retValue = clsRS::getInst().sendCommand(":METER:TRIG",true);

        double value = retValue.split(",").at(0).toDouble();

        if(qAbs(value)>5E-12)
        {
            if(isEmpty)
            {
                clsRS::getInst().sendCommand(QString(":METER:FUNC:1 %1").arg(item),false);
                emit detectInProgress(tr("已经探测到产品"));
                return true;
            }
            isEmpty=false;
        }
        else
        {
            isEmpty=true;
        }
        UserfulFunctions::sleepMs(50);
        emit detectInProgress(tr("正在探测产品"));
    }
    clsRS::getInst().sendCommand(QString(":METER:FUNC:1 %1").arg(item),false);

    emit detectInProgress("");
    return false;
}

void cls6500MeterMode::updateButtons()
{
    tabTestMode->setCurrentIndex(intSelectMode);
    btnRange->setText(range);
    btnSpeed->setText(speed);
    btnEqucct->setText(equcct);
    doubleType dt;

    if(levelType=="V")
    {
        dt.setData(levelVValue);
        btnLevel->setText(dt.formateToString()+levelType);
    }
    else
    {
        dt.setData(levelAValue);
        btnLevel->setText(dt.formateToString()+levelType);
    }

    dt.setData(frequency);
    btnFrequency->setText(dt.formateToString()+"Hz");

    if(biasType==tr("电压"))
    {
        dt.setData(biasVValue);
        btnBiasLevel->setText(dt.formateToString()+"V");
    }
    else
    {
        dt.setData(biasAValue);
        btnBiasLevel->setText(dt.formateToString()+"A");
    }

    btnBiasType->setText(biasType);
    btnBiasStatus->setText(strBiasStatus);

    btnItem1->setText(item1);
    btnItem2->setText(item2);

    btnItem1Unit->setText(item1Suffix+UserfulFunctions::getSuffix(item1));
    btnItem2Unit->setText(item2Suffix+UserfulFunctions::getSuffix(item2));

    lblItem1Limit->setText(mlItem1.showLimits(UserfulFunctions::getSuffix(item1)));
    lblItem2Limit->setText(mlItem2.showLimits(UserfulFunctions::getSuffix(item2)));

    //Res Mode
    doubleType dtRes;
    dtRes.setData(freqStart);
    btnStart->setText(dtRes.formateToString()+"Hz");

    dtRes.setData(freqStop);
    btnStop->setText(dtRes.formateToString()+"Hz");

    btnResEqucct->setText(resEqucct);
    btnResSpeed->setText(resSpeed);
    btnDepth->setText(QString::number(resDepth));

    grpFreq->setChecked(blFreq);
    grpR->setChecked(blR);
    grpC->setChecked(blC);
    grpL->setChecked(blL);

    lblFreqLimit->setText(mlFreq.showLimits(UserfulFunctions::getSuffix(tr("频率"))));
    lblRLimit->setText(mlR.showLimits(UserfulFunctions::getSuffix("R")));
    lblCLimit->setText(mlC.showLimits(UserfulFunctions::getSuffix("C")));
    lblLLimit->setText(mlL.showLimits(UserfulFunctions::getSuffix("L")));

    btnFreqUnit->setText(freqSuffix+UserfulFunctions::getSuffix(tr("频率")));
    btnRUnit->setText(rSuffix+UserfulFunctions::getSuffix("R"));
    btnCUnit->setText(cSuffix+UserfulFunctions::getSuffix("C"));
    btnLUnit->setText(lSuffix+UserfulFunctions::getSuffix("L"));

}

void cls6500MeterMode::stopDetect()
{
    isStop = false;

    qApp->processEvents();
}

void cls6500MeterMode::setCondition(QString value)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(value.toUtf8(),&error);

    if(error.error == QJsonParseError::NoError)
    {
        if(!(jsonDocument.isNull()|| jsonDocument.isEmpty()))
        {
            if(jsonDocument.isObject())
            {
                QVariantMap result = jsonDocument.toVariant().toMap();
                this->range = result["range"].toString();
                this->speed = result["speed"].toString();
                this->equcct = result["equcct"].toString();
                this->levelType = result["levelType"].toString();
                this->levelAValue = result["levelAValue"].toDouble();
                this->levelVValue = result["levelVValue"].toDouble();
                this->frequency = result["frequency"].toDouble();
                this->item1 = result["item1"].toString();
                this->item2 = result["item2"].toString();
                this->biasVValue = result["biasVValue"].toDouble();
                this->biasAValue=result["biasAValue"].toDouble();
                this->biasType=result["biasType"].toString();
                this->strBiasStatus = result["biasStatus"].toString();
                this->item1Suffix=result["item1Suffix"].toString();
                this->item2Suffix=result["item2Suffix"].toString();
                this->mlItem1.setString(result["mlItem1"].toString());
                this->mlItem2.setString(result["mlItem2"].toString());
                this->txtDescription->setText(result["txtDescription"].toString());
                this->grpTest2->setChecked(result["grpTest2"].toBool());

                //Res Mode to do test
                this->intSelectMode = result["selectMode"].toInt();
                this->freqStart = result["freqStart"].toDouble();
                this->freqStop = result["freqStop"].toDouble();
                this->resEqucct =result["resEqucct"].toString();
                this->resSpeed = result["resSpeed"].toString();
                this->resDepth = result["resDepth"].toInt();
                this->mlFreq.setString(result["mlFreq"].toString());
                this->mlR.setString(result["mlR"].toString());
                this->mlC.setString(result["mlC"].toString());
                this->mlL.setString(result["mlL"].toString());
                this->freqSuffix = result["freqSuffix"].toString();
                this->rSuffix = result["rSuffix"].toString();
                this->cSuffix = result["cSuffix"].toString();
                this->lSuffix = result["lSuffix"].toString();
                this->blFreq=result["blFreq"].toBool();
                this->blR = result["blR"].toBool();
                this->blC = result["blC"].toBool();
                this->blL = result["blL"].toBool();

                updateButtons();
            }
        }
    }
}

QString cls6500MeterMode::getConditon()
{
    QVariantMap step;


    step.insert("range",this->range);
    step.insert("speed",this->speed);
    step.insert("equcct",this->equcct);
    step.insert("levelType",this->levelType);
    step.insert("levelAValue",this->levelAValue);
    step.insert("levelVValue",this->levelVValue);
    step.insert("frequency",this->frequency);
    step.insert("item1",this->item1);
    step.insert("item2",this->item2);
    step.insert("biasVValue",this->biasVValue);
    step.insert("biasAValue",this->biasAValue);
    step.insert("biasType",this->biasType);
    step.insert("biasStatus",this->strBiasStatus);
    step.insert("item1Suffix",this->item1Suffix);
    step.insert("item2Suffix",this->item2Suffix);
    step.insert("mlItem1",this->mlItem1.toString());
    step.insert("mlItem2",this->mlItem2.toString());
    step.insert("txtDescription",this->txtDescription->text());
    step.insert("grpTest2",this->grpTest2->isChecked());
    //add 2015/10/12 Res Mode
    step.insert("selectMode",this->intSelectMode);
    step.insert("freqStart",this->freqStart);
    step.insert("freqStop",this->freqStop);
    step.insert("resEqucct",this->resEqucct);
    step.insert("resSpeed",this->resSpeed);
    step.insert("resDepth",this->resDepth);
    step.insert("mlFreq",this->mlFreq.toString());
    step.insert("mlR",this->mlR.toString());
    step.insert("mlC",this->mlC.toString());
    step.insert("mlL",this->mlL.toString());
    step.insert("freqSuffix",this->freqSuffix);
    step.insert("rSuffix",this->rSuffix);
    step.insert("cSuffix",this->cSuffix);
    step.insert("lSuffix",this->lSuffix);
    step.insert("blFreq",this->blFreq);
    step.insert("blR",this->blR);
    step.insert("blC",this->blC);
    step.insert("blL",this->blL);

    QJsonDocument jsonDocument  = QJsonDocument::fromVariant(step);

    if(!jsonDocument.isNull())
        return jsonDocument.toJson();
    else
        return "";

}

void cls6500MeterMode::updateLCRGpib()
{
    QStringList gpibCmd;
    QString meter=":METER:";

    gpibCmd.append(meter+"FREQ "+QString::number(frequency)+";");   //频率
    UserfulFunctions::sleepMs(10); //让仪器有时间更换测试频率
    if(item1==QString("θ"))
        gpibCmd.append(meter+"FUNC:1 "+"ANGLE"+";");  //item1 GPIB
    else
        gpibCmd.append(meter+"FUNC:1 "+item1+";");  //item1 GPIB

    if(item2==QString("θ"))
        gpibCmd.append(meter+"FUNC:2 "+"ANGLE"+";");  //item1 GPIB
    else
        gpibCmd.append(meter+"FUNC:2 "+item2+";");  //item1 GPIB

    if(equcct==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER"+";");
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR"+";");

    if(range ==tr("自动"))                        //档位
        gpibCmd.append(meter+"RANGE "+"AUTO"+";");
    else
        gpibCmd.append(meter+"RANGE "+range+";");

    if(speed ==tr("最快"))                        //速度
        gpibCmd.append(meter+"SPEED "+"MAX"+";");
    else if(speed ==tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST"+";");
    else if(speed ==tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED"+";");
    else if(speed ==tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW"+";");
    else
        gpibCmd.append(meter+"SPEED "+speed+";");


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
        if(biasAValue> getMaxBiasA())
            biasAValue = getMaxBiasA();

        gpibCmd.append(meter+"BIAS-TYPE "+"CUR"+";");
        gpibCmd.append(meter+"BIAS "+QString::number(biasAValue)+";");
    }

    QString biasCmd;
    if(strBiasStatus==tr("开"))
        biasCmd= meter+"BIAS-STAT "+"ON";
    else
        biasCmd=meter+"BIAS-STAT "+"OFF";

    if(clsRS::getInst().gpibCommands.gpibTest1.length() != gpibCmd.length())
    {
        for(int i=0; i< gpibCmd.length(); i++)
        {
            clsRS::getInst().sendCommand(gpibCmd.at(i),false);
            UserfulFunctions::sleepMs(50);
        }
    }
    else
    {
        for(int i=0; i< gpibCmd.length();i++)
        {
            if(clsRS::getInst().gpibCommands.gpibTest1.at(i)!= gpibCmd.at(i))
            {
                clsRS::getInst().sendCommand(gpibCmd.at(i),false);
            }
        }
    }

    clsRS::getInst().gpibCommands.gpibTest1.clear();
    clsRS::getInst().gpibCommands.gpibTest1.append(gpibCmd);

    if(clsRS::getInst().gpibCommands.biasCommand!= biasCmd)
    {
        clsRS::getInst().sendCommand(biasCmd,false);
        clsRS::getInst().gpibCommands.biasCommand= biasCmd;
    }

    if(strBiasStatus==tr("开"))
        emit biasStatus(true);
    else
        emit biasStatus(false);
}

void cls6500MeterMode::updateResGpib()
{
    QStringList gpibCmd;
    QString RESONANCE =":METER:RESO:";

    gpibCmd<<QString("%1ST %2").arg(RESONANCE,QString::number(freqStart));
    gpibCmd<<QString("%1SP %2").arg(RESONANCE,QString::number(freqStop));

    QString gpibSpeed;

    if(resSpeed ==QObject::tr("最快"))
    {
        gpibSpeed="MAX";
    }
    else if(resSpeed ==QObject::tr("快速"))
    {
        gpibSpeed="FAST";
    }
    else if(resSpeed ==QObject::tr("中速"))
    {
        gpibSpeed="MED";
    }
    else if(resSpeed ==QObject::tr("慢速"))
    {
        gpibSpeed="SLOW";
    }
    else
    {
        gpibSpeed="MAX";
    }
    gpibCmd<<QString("%1SPEED %2").arg(RESONANCE).arg(gpibSpeed);

    QString gpibEqucct;
    if(resEqucct == QObject::tr("串联"))
    {
        gpibEqucct="SER";
    }
    else if(resEqucct == QObject::tr("并联"))
    {
        gpibEqucct="PAR";
    }
    else
    {
        gpibEqucct="SER";
    }
    gpibCmd<<QString("%1EQU-CCT %2").arg(RESONANCE).arg(gpibEqucct);
    gpibCmd<<QString("%1DEPTH %2").arg(RESONANCE).arg(QString::number(resDepth));

    if(clsRS::getInst().gpibCommands.gpibTest2.length() != gpibCmd.length())
    {
        for(int i=0; i<gpibCmd.length();i++)
        {
            clsRS::getInst().sendCommand(gpibCmd.at(i),false);
        }
        clsRS::getInst().gpibCommands.gpibTest2= gpibCmd;
    }
    else
    {
        for(int i=0; i< gpibCmd.length(); i++)
        {
            if(clsRS::getInst().gpibCommands.gpibTest2.at(i)!=gpibCmd.at(i))
                clsRS::getInst().sendCommand(gpibCmd.at(i));
        }
        clsRS::getInst().gpibCommands.gpibTest2= gpibCmd;
    }
}

void cls6500MeterMode::updateGPIB()
{
    if(intSelectMode==0)
        updateLCRGpib();
    else
        updateResGpib();
}



bool cls6500MeterMode::getTotalStatus()
{
    bool status = true;

    for( int i=0; i<this->getCountTestItems();i++)
    {
        clsMeterLimit ml = getLimit(i);
        double res = getResult(i);

        status = status && ml.comparaValue(res);
    }
    return status;
}

int cls6500MeterMode::getCountTestItems()
{
    if(intSelectMode==0)
    {
        if(grpTest2->isChecked())
            return 2;
        else
            return 1;
    }
    else
    {
        int count =0;
        if(grpFreq->isChecked())
            count++;
        if(grpR->isChecked())
            count++;
        if(grpC->isChecked())
            count++;
        if(grpL->isChecked())
            count++;
        return count;
    }
}

QString cls6500MeterMode::getItem(int i)
{
    if(intSelectMode ==0)
    {
        QStringList tmp;
        tmp.append(item1);
        tmp.append(item2);
        return tmp.at(i);
    }
    else
    {
        QStringList itm;
        if(grpFreq->isChecked())
            itm.append("SRF");
        if(grpR->isChecked())
            itm.append("R");
        if(grpC->isChecked())
            itm.append("C");
        if(grpL->isChecked())
            itm.append("L");
        return itm.at(i);
    }
}

clsMeterLimit cls6500MeterMode::getLimit(int i)
{
    if(intSelectMode==0)
    {
        QList<clsMeterLimit> list;
        list.append(mlItem1);
        list.append(mlItem2);

        return list.at(i);
    }
    else
    {
        QList<clsMeterLimit> tmp;
        if(grpFreq->isChecked())
            tmp.append(mlFreq);
        if(grpR->isChecked())
            tmp.append(mlR);
        if(grpC->isChecked())
            tmp.append(mlC);
        if(grpL->isChecked())
            tmp.append(mlL);
        return tmp.at(i);
    }
}

void cls6500MeterMode::calibration()
{
    wk6500Calibration *dlg = new wk6500Calibration(getMaxFrequency(clsRS::getInst().instrumentModel));
    dlg->exec();
}

QString cls6500MeterMode::getBrief()
{
    if(intSelectMode ==0)
    {
        QStringList tmp;
        tmp.append(item1);

        if(grpTest2->isChecked())
            tmp.append(item2);

        tmp.append(btnLevel->text());
        tmp.append(btnEqucct->text());
        tmp.append(btnFrequency->text());
        tmp.append(btnBiasLevel->text());
        tmp.append(btnBiasStatus->text());
        return tmp.join(",");
    }
    else
    {
        QStringList tmp;
        tmp.append(btnResEqucct->text());
        tmp.append(btnResSpeed->text());
        tmp.append(btnStart->text());
        tmp.append(btnStop->text());
        if(grpFreq->isChecked())
            tmp.append("SRF");
        if(grpR->isChecked())
            tmp.append("R");
        if(grpC->isChecked())
            tmp.append("C");
        if(grpL->isChecked())
            tmp.append("L");
        return QString("%1:%2").arg(tr("Res. 搜索"),tmp.join(","));
    }
}

QString cls6500MeterMode::getMeterSeries()
{
    return clsRS::getInst().meterSeries;
}

QString cls6500MeterMode::getDescription()
{
    if(txtDescription->text().isEmpty())
        return tr("空白");
    else
        return txtDescription->text();
}

void cls6500MeterMode::turnOffBias()
{

    QString gpibCmd=":METER:BIAS-STAT OFF";

    if(clsRS::getInst().gpibCommands.biasCommand!=gpibCmd )
    {
        clsRS::getInst().sendCommand(gpibCmd,false);
        clsRS::getInst().gpibCommands.biasCommand=gpibCmd;
    }

}

void cls6500MeterMode::singleTrig()
{
    if(intSelectMode==0)
    {
        QString trigCmd = ":METER:TRIG";

        QString strRes = clsRS::getInst().sendCommand(trigCmd,true);
        strRes +=",,";

        item1Res= strRes.split(",").at(0).toDouble();
        item2Res = strRes.split(",").at(1).toDouble();
    }
    else
    {
        QString trigCmd = ":METER:RESO:TRIG";

        QString strRes = clsRS::getInst().sendCommand(trigCmd,true);
        strRes +=",,,,,,,";

        res1= strRes.split(",").at(0).toDouble();
        res2 = strRes.split(",").at(5).toDouble();
        res3= strRes.split(",").at(3).toDouble();
        res4 = strRes.split(",").at(4).toDouble();
    }


    QStringList showTest;
    for(int i= 0; i< getCountTestItems();i++)
    {
        QString item = getItem(i);
        double value = getResult(i);
        clsMeterLimit lm = getLimit(i);
        QString suffix = getSuffix(i);
        QString toShow =getItemShow(item,value,lm,suffix);
        showTest.append( toShow);
    }
    emit signalTestResult(showTest.join("|"));
}

QString cls6500MeterMode::getItemShow(const QString &item, const double &value , clsMeterLimit &limit, const QString &/*suffix*/)
{
    QStringList tmp;
    tmp<< item;

    doubleType dt;
    dt.setData(value,"");


    bool blValue = limit.comparaValue(value);
    QString strPassFail = blValue?tr("PASS"):tr("FAIL");

    if((item=="Q") || (item=="D"))
    {
        tmp.append(dt.formateWithUnit(" ",7)+UserfulFunctions::getSuffix(item));
    }
    else
    {
        tmp.append(dt.formateToString(7)+UserfulFunctions::getSuffix(item));
    }
    tmp.append(strPassFail);

    return tmp.join("|");

}
//用于产品测试。
void cls6500MeterMode::repetiveTrig()
{
    if(intSelectMode == 0)
    {
        QString trigCmd = ":METER:TRIG";

        QString strRes = clsRS::getInst().sendCommand(trigCmd,true);
        strRes +=",,";

        item1Res= strRes.split(",").at(0).toDouble();
        item2Res = strRes.split(",").at(1).toDouble();
    }
    else
    {
        QString trigCmd = ":METER:RESO:TRIG";

        QString strRes = clsRS::getInst().sendCommand(trigCmd,true,13);
        strRes +=",,,,,,,";

        res1= strRes.split(",").at(0).toDouble();
        res2 = strRes.split(",").at(5).toDouble();
        res3= strRes.split(",").at(3).toDouble();
        res4 = strRes.split(",").at(4).toDouble();
    }

}

double cls6500MeterMode::getResult(int i)
{
    if(intSelectMode ==0)
    {
        QList<double> tmp;
        tmp.append(item1Res);
        tmp.append(item2Res);
        return tmp.at(i);
    }
    else
    {
        QList<double> tmp;
        if(grpFreq->isChecked())
            tmp.append(res1);
        if(grpR->isChecked())
            tmp.append(res2);
        if(grpC->isChecked())
            tmp.append(res3);
        if(grpL->isChecked())
            tmp.append(res4);
        return tmp.at(i);
    }
}

QString cls6500MeterMode::getSuffix(int i)
{
    if(intSelectMode ==0)
    {
        QStringList tmp;
        tmp.append(item1Suffix);
        tmp.append(item2Suffix);
        return tmp.at(i);
    }
    else
    {
        QList<QString> tmp;
        if(grpFreq->isChecked())
            tmp.append(freqSuffix);
        if(grpR->isChecked())
            tmp.append(rSuffix);
        if(grpC->isChecked())
            tmp.append(cSuffix);
        if(grpL->isChecked())
            tmp.append(lSuffix);
        return tmp.at(i);
    }
}

QString cls6500MeterMode::getFreq()
{
    if(intSelectMode==0)
        return this->btnFrequency->text();
    else
        return "N/A";
}

QString cls6500MeterMode::getEqucct()
{
    if(intSelectMode==0)
        return this->btnEqucct->text();
    else
        return this->resEqucct;
}

QString cls6500MeterMode::getLevel()
{
    return this->btnLevel->text();
}


void cls6500MeterMode::on_btnRange_clicked()
{
    wk6500Range * dialog= new wk6500Range;
    dialog->setWindowTitle(tr("设置6500的测试档位"));

    if(dialog->exec()==QDialog::Accepted)
    {
        range = dialog->getRange();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnSpeed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed;
    dlg->setWindowTitle(tr("设置测试速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        speed = dlg->getSpeed();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnEqucct_clicked()
{
    equcct = equcct==tr("串联")? tr("并联"):tr("串联");
    updateButtons();
}

void cls6500MeterMode::on_btnFrequency_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设定测试频率"));
    dlg->setValueAndSuffix(frequency,"");

    if(dlg->exec()==QDialog::Accepted)
    {
        frequency = dlg->getNumber();
        frequency =(frequency<20? 20:frequency);
        double maxFreq =getMaxFrequency(clsRS::getInst().instrumentModel);
        frequency =(frequency>maxFreq?maxFreq:frequency);
        updateButtons();
    }
}

double cls6500MeterMode::getMaxFrequency(QString value)
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

void cls6500MeterMode::on_btnLevel_clicked()
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

        updateButtons();
    }
}

void cls6500MeterMode::on_btnBiasLevel_clicked()
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
        }
        else
        {
            double maxC = getMaxBiasA();
            biasAValue =value;
            biasAValue=(biasAValue<0?0:biasAValue);
            biasAValue=(biasAValue>maxC?maxC:biasAValue);
            dt.setData(biasAValue,"");
        }
        updateButtons();
    }
}

void cls6500MeterMode::on_btnBiasType_clicked()
{
    biasType = biasType==tr("电压")?tr("电流"):tr("电压");
    updateButtons();
}

void cls6500MeterMode::on_btnBiasStatus_clicked()
{
    strBiasStatus = strBiasStatus==tr("开")?tr("关"):tr("开");
    updateButtons();
}

void cls6500MeterMode::on_btnItem1_clicked()
{
    dlgFunction *dlg = new dlgFunction;
    dlg->setWindowTitle(tr("设置6500的测试项目1"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item1 = dlg->getItem();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnItem2_clicked()
{
    dlgFunction *dlg = new dlgFunction;
    dlg->setWindowTitle(tr("设置6500的测试项目2"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item2 = dlg->getItem();

        updateButtons();
    }
}

void cls6500MeterMode::on_btnItem1Unit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->item1Suffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnItem2Unit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->item2Suffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls6500MeterMode::lblLmItem1()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlItem1);
    dlg->setItem(item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        mlItem1 = dlg->getMeterLimit();
        updateButtons();
    }

}

void cls6500MeterMode::lblLmItem2()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlItem2);
    dlg->setItem(item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        mlItem2 = dlg->getMeterLimit();
        updateButtons();
    }

}

void cls6500MeterMode::lblLmFreq()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlFreq);
    dlg->setItem(tr("频率"));
    if(dlg->exec()==QDialog::Accepted)
    {
        mlFreq = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6500MeterMode::lblLmR()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlR);
    dlg->setItem("R");
    if(dlg->exec()==QDialog::Accepted)
    {
        mlR = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6500MeterMode::lblLmC()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlC);
    dlg->setItem("C");
    if(dlg->exec()==QDialog::Accepted)
    {
        mlC = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6500MeterMode::lblLmL()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlL);
    dlg->setItem("L");
    if(dlg->exec()==QDialog::Accepted)
    {
        mlL = dlg->getMeterLimit();
        updateButtons();
    }
}

double cls6500MeterMode::getMaxBiasA()
{
    static double maxC =0;

    if(maxC==0)
    {
        clsRS::getInst().sendCommand("*SYSBIAS \'BIAS EXT\'",false);
        if(clsRS::getInst().sendCommand("*SYSBIAS?",true)=="BIAS EXT")
        {

            double maxFreq =getMaxFrequency(clsRS::getInst().instrumentModel);
            if(maxFreq>15E9)
                maxC=40.0;
            else
                maxC=50.0;

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

void cls6500MeterMode::on_tabTestMode_currentChanged(int index)
{
    intSelectMode = index;
    //qDebug()<< index;
}

void cls6500MeterMode::on_grpFreq_toggled(bool arg1)
{
    blFreq = arg1;
}

void cls6500MeterMode::on_grpR_toggled(bool arg1)
{
    blR = arg1;
}

void cls6500MeterMode::on_grpC_toggled(bool arg1)
{
    blC = arg1;
}

void cls6500MeterMode::on_grpL_toggled(bool arg1)
{
    blL = arg1;
}

void cls6500MeterMode::on_btnFreqUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(tr("频率"));
    if(dlg->exec()==QDialog::Accepted)
    {
        this->freqSuffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnRUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("R");
    if(dlg->exec()==QDialog::Accepted)
    {
        this->rSuffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnCUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("C");
    if(dlg->exec()==QDialog::Accepted)
    {
        this->cSuffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnLUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("L");
    if(dlg->exec()==QDialog::Accepted)
    {
        this->lSuffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnStart_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设定起始频率"));
    dlg->setValueAndSuffix(freqStart,"");

    if(dlg->exec()==QDialog::Accepted)
    {
        freqStart = dlg->getNumber();
        freqStart =(freqStart<20? 20:freqStart);
        double maxFreq =getMaxFrequency(clsRS::getInst().instrumentModel);
        freqStart =(freqStart>maxFreq?maxFreq:freqStart);
        updateButtons();
    }
}

void cls6500MeterMode::on_btnStop_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设定终止频率"));
    dlg->setValueAndSuffix(freqStop,"");

    if(dlg->exec()==QDialog::Accepted)
    {
        freqStop = dlg->getNumber();
        freqStop =(freqStop<20? 20:freqStop);
        double maxFreq =getMaxFrequency(clsRS::getInst().instrumentModel);
        freqStop =(freqStop>maxFreq?maxFreq:freqStop);
        updateButtons();
    }
}

void cls6500MeterMode::on_btnDepth_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设定扫描深度(0-16)"));
    dlg->setValueAndSuffix(resDepth,"");

    if(dlg->exec()==QDialog::Accepted)
    {
        resDepth = (int)dlg->getNumber();
        resDepth =(resDepth<0? 0:resDepth);
        resDepth =(resDepth>16?16:resDepth);
        updateButtons();
    }
}

void cls6500MeterMode::on_btnResSpeed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed;
    dlg->setWindowTitle(tr("设置搜索速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        resSpeed = dlg->getSpeed();
        updateButtons();
    }
}

void cls6500MeterMode::on_btnResEqucct_clicked()
{
    if( resEqucct == tr("串联"))
    {
        resEqucct = tr("并联");
    }
    else
    {
        resEqucct = tr("串联");
    }

    updateButtons();
}
