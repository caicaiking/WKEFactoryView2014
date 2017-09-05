#include "cls3260MeterMode.h"
#include <QDebug>
#include "cls3260minorfunction.h"
#include "cls3260majorfunction.h"
#include "frmSetLimit.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "clsMeterUnit.h"
#include "clsRuningSettings.h"
#include "dlgLevelInputBox.h"
#include "NumberInput.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "cls3260Calibration.h"
#include "frmWK3260Calibration.h"
#include "cls6440Calibration.h"
#include <QJsonDocument>
cls3260MeterMode::cls3260MeterMode(WKEMeterMode *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);

    connect(lblMaijor,SIGNAL(Clicked()),this,SLOT(limitMajorClick()));
    connect(lblMinor,SIGNAL(Clicked()),this,SLOT(limitMinorClick()));
    connect(lblRdc,SIGNAL(Clicked()),this,SLOT(limitRdcClick()));

    strDescription = tr("空白");
    item1="L";
    item2="Q";
    enableMinor=true;
    level =0.1;
    alcStatus = "ALC OFF";
    levelType="V";
    frequency=100000;
    blBiasStatus= false;
    range = tr("自动");
    rangeRdc = tr("自动");
    speed = tr("最快");
    equcct = tr("串联");
    levelRdc = 1;
    majorUnit="";
    minorUnit="";
    rdcUnit="";

    biasType = tr("Norm");
    biasValue = 0.0;
    biasSpeed =tr("OFF");
    blBiasStatus = false;

    blStop = false;

    updateButtons();
}


bool cls3260MeterMode::detectDut()
{
    blStop= true;
    bool isEmpty=false;
    QString meter =getMeter();

    QString item;

    if(clsRS::getInst().gpibCommands.testMode=="AC")
    {
        int index = clsRS::getInst().sendCommand(meter+":FUNC:MAJOR?",true).toInt();

        switch (index) {
        case 0:
            item="L";
            break;
        case 1:
            item="C";
            break;
        case 2:
            item="Z";
            break;
        default:
            break;
        }

        if(item!="C")
            clsRS::getInst().sendCommand(QString(meter+":FUNC:C"),false);
    }

    while(blStop)
    {


        if(clsRS::getInst().gpibCommands.testMode=="RDC")
        {
            QString retValue = clsRS::getInst().sendCommand(meter+":TRIG",true);

            double value = retValue.toDouble();

            if(qAbs(value)<1E6)
            {
                if(isEmpty)
                {
                    emit detectInProgress(tr("已经探测到产品"));
                    return true;
                }
                isEmpty=false;
            }
            else
            {
                isEmpty=true;
            }
        }
        else
        {
            QString retValue = clsRS::getInst().sendCommand(meter+":TRIG",true);

            double value = retValue.split(",").at(0).toDouble();

            if(qAbs(value)>50E-12)
            {
                if(isEmpty)
                {
                    clsRS::getInst().sendCommand(QString(meter+":FUNC:%1").arg(item),false);
                    emit detectInProgress(tr("已经探测到产品"));
                    return true;
                }
                isEmpty=false;
            }
            else
            {
                isEmpty=true;
            }
        }
        UserfulFunctions::sleepMs(50);

        emit detectInProgress(tr("正在探测产品"));
    }
    clsRS::getInst().sendCommand(QString(":MEAS:FUNC:%1").arg(item),false);

    emit detectInProgress("");
    return false;
}

void cls3260MeterMode::stopDetect()
{
    blStop = false;
    qApp->processEvents();
}

void cls3260MeterMode::setCondition(QString value)
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
                tabMeter->setCurrentIndex(result["currentIndex"].toInt());
                this->frequency= result["frequency"].toDouble();
                this->level = result["level"].toDouble();
                this->levelType = result["leveType"].toString();
                this->equcct = result["equcct"].toString();
                this->range = result["range"].toString();
                this->speed = result["speed"].toString();
                this->alcStatus = result["alcStatus"].toString();

                //Bias
                this->biasType = result["biasType"].toString();
                this->biasSpeed = result["biasSpeed"].toString();
                this->biasValue = result["biasValue"].toDouble();
                this->blBiasStatus = result["blBiasStatus"].toBool();

                this->item1 = result["item1"].toString();
                this->item2 = result["item2"].toString();
                this->enableMinor = result["enableMinor"].toBool();
                this->lmMajor.setString(result["lmMajor"].toString());
                this->lmMinor.setString(result["lmMinor"].toString());
                this->majorUnit = result["majorSuffix"].toString();
                this->minorUnit = result["minorSuffix"].toString();
                this->levelRdc = result["rdcLevel"].toDouble();
                this->rangeRdc = result["rdcRange"].toString();
                this->lmRdc.setString(result["lmRdc"].toString());
                this->rdcUnit = result["rdcSuffix"].toString();
                this->strDescription= result["description"].toString();
                txtBiasOn->setValue(result["biasOnTime"].toInt());
                txtBiasOff->setValue(result["biasOffTime"].toInt());
                updateButtons();
            }
        }
    }
}

QString cls3260MeterMode::getConditon()
{
    QVariantMap step;
    step.insert("currentIndex",tabMeter->currentIndex());
    step.insert("frequency",this->frequency);
    step.insert("level",this->level);
    step.insert("leveType",this->levelType);
    step.insert("equcct",this->equcct);
    step.insert("range",this->range);
    step.insert("speed",this->speed);
    step.insert("alcStatus",this->alcStatus);

    //Insert Bias
    step.insert("biasType",this->biasType);
    step.insert("biasSpeed",this->biasSpeed);
    step.insert("biasValue",this->biasValue);
    step.insert("blBiasStatus",this->blBiasStatus);


    step.insert("item1",this->item1);
    step.insert("item2",this->item2);
    step.insert("lmMajor",this->lmMajor.toString());
    step.insert("lmMinor",this->lmMinor.toString());
    step.insert("majorSuffix",this->majorUnit);
    step.insert("minorSuffix",this->minorUnit);
    step.insert("enableMinor",this->enableMinor);
    step.insert("rdcLevel",this->levelRdc);
    step.insert("rdcRange",this->rangeRdc);
    step.insert("lmRdc",this->lmRdc.toString());
    step.insert("rdcSuffix",this->rdcUnit);

    step.insert("description",this->strDescription);
    step.insert("biasOnTime",this->txtBiasOn->value());
    step.insert("biasOffTime",this->txtBiasOff->value());

    QJsonDocument jsonDocument  = QJsonDocument::fromVariant(step);

    if(!jsonDocument.isNull())
        return jsonDocument.toJson();
    else
        return "";
}



void cls3260MeterMode::updateGPIB()
{
    QString meter=getMeter();
    if(tabMeter->currentIndex()==0)
    {
        if(clsRS::getInst().gpibCommands.testMode !="AC")
        {
            clsRS::getInst().sendCommand(meter,false);
            clsRS::getInst().sendCommand(meter+":TEST",false);
            clsRS::getInst().sendCommand(meter+":TEST:AC",false);
            clsRS::getInst().gpibCommands.testMode ="AC";
        }
        QStringList gpibCmd;

        // QString meter2="MEAS:";

        if(equcct==tr("串联"))
            gpibCmd.append(meter+":EQU-CCT "+"SER");
        else
            gpibCmd.append(meter+":EQU-CCT "+"PAR");



        if(item1 !="Z" && item1 !="Y")
        {
            gpibCmd.append(meter+":FUNC:"+item1 +";" +item2);
        }
        else
        {
            gpibCmd.append(meter+":FUNC:"+ item1 );  //item1 GPIB
        }

        if(range==tr("自动"))
            gpibCmd.append(meter+":RANGE "+"AUTO");
        else
            gpibCmd.append(meter+":RANGE "+range);

        if(speed==tr("最快"))
            gpibCmd.append(meter+":SPEED "+"MAX");
        else if(speed==tr("快速"))
            gpibCmd.append(meter+":SPEED "+"FAST");
        else if(speed==tr("中速"))
            gpibCmd.append(meter+":SPEED "+"MED");
        else if(speed==tr("慢速"))
            gpibCmd.append(meter+":SPEED "+"SLOW");
        else
            gpibCmd.append(meter+":SPEED "+"MAX");

        gpibCmd.append(meter+":FREQ "+QString::number(frequency));

        gpibCmd.append(meter + ":"+ alcStatus);
        if(levelType=="V")
        {
            gpibCmd.append(meter+":LEV "+QString::number(level)+"V");
        }
        else
        {
            gpibCmd.append(meter+":LEV "+QString::number(level)+"A");
        }

        if(biasType == tr("Norm"))
            gpibCmd.append(meter+":BIAS INT");
        else
            gpibCmd.append(meter+":BIAS EEXT");

        gpibCmd.append(meter+":I-SWEEP "+biasSpeed.toUpper());
        gpibCmd.append(meter+":BIAS "+ QString::number(this->biasValue));

        //change Bias condition to GPIB commands.


        //        if(biasSource==tr("内置"))
        //        {
        //            gpibCmd.append(meter+"BIAS "+"VINT");
        //        }
        //        else
        //        {
        //            gpibCmd.append(meter+"BIAS "+"VEXT");
        //        }



        if(clsRS::getInst().gpibCommands.gpibTest1.length() != gpibCmd.length())
        {

            clsRS::getInst().gpibCommands.gpibTest1.clear();


            for(int i =0; i< gpibCmd.length(); i++)
            {
                clsRS::getInst().sendCommand(gpibCmd.at(i));
                //qDebug()<< gpibCmd.at(i);
                UserfulFunctions::sleepMs(50);
            }


        }
        else
        {
            for(int i =0; i<gpibCmd.length();i++)
            {
                if(clsRS::getInst().gpibCommands.gpibTest1.at(i)!= gpibCmd.at(i))
                {
                    clsRS::getInst().sendCommand(gpibCmd.at(i));
                    //qDebug()<<gpibCmd.at(i);
                    UserfulFunctions::sleepMs(50);
                }
            }
        }

        clsRS::getInst().gpibCommands.gpibTest1 = gpibCmd;

        QString tmpBiasString;
        if(!blBiasStatus)
        {
            tmpBiasString=meter+":BIAS "+"OFF";
        }
        else
        {
            tmpBiasString=meter+":BIAS "+"ON";
        }
        if(clsRS::getInst().gpibCommands.biasCommand != tmpBiasString)
        {
            clsRS::getInst().sendCommand(tmpBiasString,false);
            clsRS::getInst().gpibCommands.biasCommand = tmpBiasString;
        }
        emit biasStatus(blBiasStatus);




        //        QString xx =QString(meter+":FREQ?");
        //        QString retFreq = clsRS::getInst().sendCommand(xx,true);
        //        this->frequency = retFreq.toDouble();
        UserfulFunctions::sleepMs(10);
        updateButtons();
    }

    else
    {
        if(clsRS::getInst().gpibCommands.testMode !="RDC")
        {
            clsRS::getInst().sendCommand(meter,false);
            clsRS::getInst().sendCommand(meter+":TEST",false);
            clsRS::getInst().sendCommand(meter+":TEST:RDC",false);
            clsRS::getInst().gpibCommands.testMode ="RDC";
        }

        QStringList gpibCmd;

        if(speed==tr("最快"))
            gpibCmd.append(meter+":SPEED "+"MAX");
        else if(speed==tr("快速"))
            gpibCmd.append(meter+":SPEED "+"FAST");
        else if(speed==tr("中速"))
            gpibCmd.append(meter+":SPEED "+"MED");
        else if(speed==tr("慢速"))
            gpibCmd.append(meter+":SPEED "+"SLOW");
        else
            gpibCmd.append(meter+":SPEED "+"MAX");

        if(range==tr("自动"))
            gpibCmd.append(meter+":RANGE "+"AUTO");
        else
            gpibCmd.append(meter+":RANGE "+range);
        gpibCmd.append(meter+":LEV "+QString::number(levelRdc)+"V");

        if(clsRS::getInst().gpibCommands.gpibTest2.length()!= gpibCmd.length())
        {
            clsRS::getInst().gpibCommands.gpibTest2.clear();

            for(int i=0; i< gpibCmd.length();i++)
            {
                clsRS::getInst().sendCommand(gpibCmd.at(i),false);
                UserfulFunctions::sleepMs(50);

            }
        }
        else
        {
            for(int i=0; i< gpibCmd.length();i++)
            {
                if(clsRS::getInst().gpibCommands.gpibTest2.at(i)!= gpibCmd.at(i))
                {
                    clsRS::getInst().sendCommand(gpibCmd.at(i),false);
                    UserfulFunctions::sleepMs(50);
                }
            }
        }

        clsRS::getInst().gpibCommands.gpibTest2 = gpibCmd;
        clsRS::getInst().gpibCommands.biasCommand = meter+":BIAS OFF";
        emit biasStatus(false);
    }

}

bool cls3260MeterMode::getTotalStatus()
{
    bool tmp = true;

    if(tabMeter->currentIndex()==1)
    {
        return lmRdc.comparaValue(dblRdc);
    }

    tmp = lmMajor.comparaValue(this->dblItem1);
    if(grpMinor->isChecked())
        tmp = tmp && lmMinor.comparaValue(dblItem2);

    return tmp;

}

int cls3260MeterMode::getCountTestItems()
{
    if(tabMeter->currentIndex()==0)
    {
        return 1+(grpMinor->isChecked()?1:0);
    }
    else if(tabMeter->currentIndex()==1)
    {
        return 1;
    }
    else
        return 0;
}

QString cls3260MeterMode::getItem(int i)
{
    if(tabMeter->currentIndex()==1)
        return "Rdc";
    else if(tabMeter->currentIndex()==0)
    {
        if(grpMinor->isChecked())
        {
            QStringList tmp;
            tmp << item1<< item2;
            return tmp.at(i);
        }
        else
            return item1;
    }
}

QString cls3260MeterMode::getSuffix(int i)
{
    if(tabMeter->currentIndex()==1)
    {
        return rdcUnit;
    }
    else
    {
        if(grpMinor->isChecked())
        {
            QStringList tmp;
            tmp << majorUnit<< minorUnit;
            return tmp.at(i);
        }
        else
            return majorUnit;
    }
}

QString cls3260MeterMode::getFreq()
{
    return this->btnFrequency->text();
}

QString cls3260MeterMode::getEqucct()
{
    return this->btnEqucct->text();
}

QString cls3260MeterMode::getLevel()
{
    return this->btnLevel->text();
}

QString cls3260MeterMode::getBias()
{
    return this->btnBiasValue->text() + " " + btnBiasOnOFF->text();
}

void cls3260MeterMode::setItemValue(SweepType t, double value)
{
    switch (t) {
    case Frequency:
        this->frequency = value;
        break;
    case BiasA:
        this->biasValue = value;
        break;

    case levelA:
        this->level = value;
        this->levelType = "A";
        break;

    case levelV:
        this->level = value;
        this->levelType = "V";
        break;
    default:
        break;
    }

    updateButtons();
}

double cls3260MeterMode::getResult(int i)
{
    if(tabMeter->currentIndex()==0)
    {
        if(grpMinor->isChecked())
        {
            QList<double> tmp;
            tmp.append(dblItem1);
            tmp.append(dblItem2);
            if(i>= tmp.length())
                return 0;
            else
                return tmp.at(i);
        }
        else
            return dblItem1;
    }
    else
        return dblRdc;

}

clsMeterLimit cls3260MeterMode::getLimit(int i)
{
    if(tabMeter->currentIndex()==1)
        return lmRdc;
    else if(tabMeter->currentIndex()==0)
    {
        if(grpMinor->isChecked())
        {
            QList<clsMeterLimit> tmp;
            tmp << lmMajor<< lmMinor;
            return tmp.at(i);
        }
        else
            return lmMajor;
    }
}

void cls3260MeterMode::calibration()
{

    frmWK3260Calibration *dlg = new frmWK3260Calibration(this);
    dlg->setWindowTitle(tr("仪表校准"));
    dlg->exec();
}

QString cls3260MeterMode::getBrief()
{
    QString tmp;

    if(tabMeter->currentIndex()==0)
    {
        tmp.append(item1);
        tmp.append(",");
        if(grpMinor->isChecked())
        {
            tmp.append(item2);
            tmp.append(",");
        }

        tmp.append(btnFrequency->text()+",");
        tmp.append(btnEqucct->text()+",");
        tmp.append(cmbBiasChioce->currentText()+",");
        tmp.append(btnBiasValue->text()+",");
        tmp.append(btnBiasOnOFF->text());
    }
    else
    {
        tmp.append("Rdc");
        tmp.append(",");
        tmp.append(btnRdcLevel->text()+",");
        tmp.append(btnRdcSpeed->text()+",");
        tmp.append(btnRdcRange->text());
    }

    return tmp;
}

QString cls3260MeterMode::getMeterSeries()
{
    return clsRS::getInst().meterSeries;
}

QString cls3260MeterMode::getDescription()
{
    return this->strDescription;
}

void cls3260MeterMode::turnOffBias()
{
    QString meter = getMeter();
    if(clsRS::getInst().gpibCommands.testMode=="RDC")
    {
        emit biasStatus(false);
        return;
    }
    else
    {
        if(clsRS::getInst().gpibCommands.biasCommand==meter+":BIAS OFF")
        {
            emit biasStatus(false);
        }
        else
        {

            clsRS::getInst().sendCommand(meter+":BIAS OFF",false);
            clsRS::getInst().gpibCommands.biasCommand = meter+":BIAS OFF";
            emit biasStatus(false);
        }
    }
}

void cls3260MeterMode::singleTrig()
{

    QString trigCmd =getMeter()+":TRIG";

    if(tabMeter->currentIndex()==0)
    {
        if(btnBiasOnOFF->text()==tr("开") && (txtBiasOn->value() !=0))
        {
            txtStatus->setText(tr("bias持续"));
            UserfulFunctions::sleepMs(txtBiasOn->value());
        }
    }


    QString strRes = clsRS::getInst().sendCommand(trigCmd,true);

    strRes+=",,";

    QStringList lsRes = strRes.split(",");

    if(tabMeter->currentIndex()==0)
    {
        dblItem1 = lsRes.at(0).toDouble();
        dblItem2 = lsRes.at(1).toDouble();

        QString tmp = getItemShow(item1,dblItem1,lmMajor,majorUnit);

        if(grpMinor->isChecked())
        {
            tmp+="|";
            tmp+= getItemShow(item2,dblItem2,lmMinor,minorUnit);
        }


        emit signalTestResult(tmp);


        if(btnBiasOnOFF->text()==tr("开") && (txtBiasOff->value() !=0))
        {
            this->turnOffBias();
            txtStatus->setText(tr("bias休息"));
            UserfulFunctions::sleepMs(txtBiasOff->value());
        }
        txtStatus->setText(tr("---"));
    }
    else
    {
        dblRdc = lsRes.at(0).toDouble();
        QString tmp = getItemShow("RDC",dblRdc,lmRdc,rdcUnit);
        emit signalTestResult(tmp);
    }

}

QString cls3260MeterMode::getItemShow(const QString &item, const double &value , clsMeterLimit &limit, const QString &/*suffix*/)
{
    QStringList tmp;
    tmp<< item;

    doubleType dt;
    dt.setData(value,"");


    bool blValue = limit.comparaValue(value);
    QString strPassFail = blValue?tr("PASS"):tr("FAIL");

    if((item=="Q") || (item=="D"))
    {
        tmp.append(dt.formateWithUnit("",7)+UserfulFunctions::getSuffix(item));
    }
    else
    {
        tmp.append(dt.formateToString(7)+UserfulFunctions::getSuffix(item));
    }
    tmp.append(strPassFail);

    return tmp.join("|");

}

void cls3260MeterMode::repetiveTrig()
{
    QString trigCmd = getMeter()+":TRIG";
    if(tabMeter->currentIndex()==0)
    {
        if(btnBiasOnOFF->text()==tr("开") && (txtBiasOn->value() !=0))
        {
            txtStatus->setText(tr("bias持续"));
            UserfulFunctions::sleepMs(txtBiasOn->value());
        }
    }
    QString strRes = clsRS::getInst().sendCommand(trigCmd,true);

    strRes+=",,";
    QStringList lsRes = strRes.split(",");
    if(tabMeter->currentIndex()==0)
    {
        dblItem1 = lsRes.at(0).toDouble();
        dblItem2 = lsRes.at(1).toDouble();
    }
    else
    {
        dblRdc = lsRes.at(0).toDouble();
    }
    if(tabMeter->currentIndex()==0)
    {
        if(btnBiasOnOFF->text()==tr("开") && (txtBiasOff->value() !=0))
        {
            this->turnOffBias();
            txtStatus->setText(tr("bias休息"));
            UserfulFunctions::sleepMs(txtBiasOff->value());
        }
        txtStatus->setText(tr("---"));
    }
}


void cls3260MeterMode::limitMajorClick()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(this->lmMajor);
    dlg->setWindowTitle(tr("设置主参数上下限"));
    dlg->setItem(item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->lmMajor = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls3260MeterMode::limitMinorClick()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(this->lmMinor);
    dlg->setWindowTitle(tr("设置副参数上下限"));
    dlg->setItem(item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->lmMinor = dlg->getMeterLimit();
        updateButtons();
    }

}

void cls3260MeterMode::limitRdcClick()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(this->lmRdc);
    dlg->setWindowTitle(tr("设置Rdc上下限"));
    dlg->setItem("RDC");
    if(dlg->exec()==QDialog::Accepted)
    {
        this->lmRdc = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls3260MeterMode::updateButtons()
{
    btnItem1->setText(item1);
    btnItem2->setText(item2);
    QString zy ="ZY";
    if(zy.contains(item1))
    {
        item2 = "θ";
        btnItem2->setText(item2);
    }

    level = getOkLevel(level, levelType);
    doubleType dt;
    dt.setData(level,"");


    grpMinor->setChecked(enableMinor);
    btnLevel->setText(dt.formateToString()+levelType);

    dt.setData(frequency);
    btnFrequency->setText(dt.formateToString()+"Hz");

    //updata Bias buttons and cmbox;
    setCmbboxSelected(this->cmbBiasChioce,this->biasType);
    setCmbboxSelected(this->cmbAddBiasSpeed,this->biasSpeed);
    btnBiasOnOFF->setText(blBiasStatus?tr("开"):tr("关"));
    dt.setData(biasValue);
    btnBiasValue->setText(dt.formateToString(6)+"A");



    // btnRange->setText(range);
    btnSpeed->setText(speed);
    btnEqucct->setText(equcct);

    lblMaijor->setText(lmMajor.showLimits(UserfulFunctions::getSuffix(item1)));
    lblMinor->setText(lmMinor.showLimits(UserfulFunctions::getSuffix(item2)));
    lblRdc->setText(lmRdc.showLimits(UserfulFunctions::getSuffix(QString("RDC"))));

    dt.setData(levelRdc);
    btnRdcLevel->setText(dt.formateToString(6)+"V");

    btnMajorUnit->setText(majorUnit+UserfulFunctions::getSuffix(item1));
    btnMinorUnit->setText(minorUnit+UserfulFunctions::getSuffix(item2));
    btnRdcUnit->setText(rdcUnit+UserfulFunctions::getSuffix(QString("RDC")));

    btnRdcSpeed->setText(this->speed);
    btnRdcRange->setText(this->rangeRdc);
    btnALC->setText(this->alcStatus);

    this->txtDescription->setText(this->strDescription);

   QString strTitle = (clsRS::getInst().meterSeries =="3260"? tr("WK 3260 Meter"): tr("WK 3255 Meter"));
    lblTitle->setText(strTitle);
}

void cls3260MeterMode::setCmbboxSelected(QComboBox *cmb, QString tmp)
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

void cls3260MeterMode::on_btnMajorUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(this->item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        majorUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls3260MeterMode::on_btnMinorUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(this->item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        minorUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls3260MeterMode::on_btnRdcUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("RDC");
    if(dlg->exec()==QDialog::Accepted)
    {
        rdcUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls3260MeterMode::on_btnFrequency_clicked()
{

    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入测试频率"));

    doubleType dt;
    dt.setData(this->frequency);

    double tmp;
    QString suffix;

    dt.getDataAndUnit(tmp,suffix);
    dlg->setValueAndSuffix(tmp,suffix);


    if(dlg->exec()==QDialog::Accepted)
    {
        tmp = dlg->getNumber();

        tmp = tmp>getMaxFreq()? getMaxFreq(): tmp;
        tmp = tmp<20.0? 20: tmp;

        this->frequency = tmp;
        updateButtons();
    }

}

double cls3260MeterMode::getMaxFreq()
{
    if(clsRS::getInst().meterSeries=="3260")
        return 3000000;

    if(clsRS::getInst().meterSeries=="3255" && clsRS::getInst().instrumentModel.contains("BQ"))
        return 1000000;

    if(clsRS::getInst().meterSeries=="3255" && clsRS::getInst().instrumentModel.contains("BL"))
        return 200000;

    if(clsRS::getInst().meterSeries=="3255" && clsRS::getInst().instrumentModel.contains("B"))
        return 500000;

    return 3000000;

}

void cls3260MeterMode::on_btnLevel_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox(this);
    dlg->setWindowTitle(tr("输入测试电平"));

    if(dlg->exec() == QDialog::Accepted)
    {
        double tmpValue = dlg->getValue();
        QString tmpSuffix = dlg->getSuffix();
        QString tmpUnit = dlg->getUnit();

        doubleType dt;
        dt.setData(tmpValue,tmpSuffix);


        this->level = getOkLevel(dt.Data(),tmpUnit);
        this->levelType = tmpUnit;
        updateButtons();

    }
}

double cls3260MeterMode::getOkLevel(double value, QString unit)
{
    if(unit =="V")
    {
        value = value>10.0? 10.0: value;
        value = value <0.001? 0.001 : value;
    }
    else
    {
        value = value> 0.2 ? 0.2: value;
        value = value<0.0005 ? 0.0005 : value;
    }

    return value;
}

void cls3260MeterMode::on_btnEqucct_clicked()
{
    if(this->equcct ==tr("串联"))
        this->equcct = tr("并联");
    else
        this->equcct =tr("串联") ;
    updateButtons();

}



void cls3260MeterMode::on_btnSpeed_clicked()
{
    dlgSpeed *dlg=new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        speed=dlg->getSpeed();
        updateButtons();
    }
}



void cls3260MeterMode::on_btnItem1_clicked()
{
    cls3260MajorFunction *dlg=new cls3260MajorFunction();
    dlg->setWindowTitle(tr("设置测试项目1"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item1=dlg->getItem();
        this->btnItem1->setText(item1);

        if(item1=="Z"|| item1=="Y")
        {
            this->btnItem2->setText("θ");
            this->item2="θ";
            updateButtons();
            return ;
        }

        if(this->btnItem2->text()=="θ")
        {
            this->btnItem2->setText("Q");
            item2 = "Q";
            updateButtons();
        }
    }
}

void cls3260MeterMode::on_btnItem2_clicked()
{
    if(this->btnItem1->text()=="Z"|| btnItem1->text()=="Y")
    {
        this->btnItem2->setText("θ");
        this->item2="θ";
        updateButtons();
        return;
    }

    cls3260MinorFunction *dlg = new cls3260MinorFunction();
    dlg->setWindowTitle(tr("设置测试项目2"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item2 = dlg->getItem();
        updateButtons();
    }
}


void cls3260MeterMode::on_btnRdcRange_clicked()
{
    wk6500Range *dlg=new wk6500Range;
    dlg->setWindowTitle(tr("设置档位"));

    if(dlg->exec()==QDialog::Accepted)
    {
        rangeRdc=dlg->getRange();
        updateButtons();
    }
}

void cls3260MeterMode::on_btnRdcSpeed_clicked()
{
    dlgSpeed *dlg=new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        speed=dlg->getSpeed();
        updateButtons();
    }
}

void cls3260MeterMode::on_btnRdcLevel_clicked()
{
    if(levelRdc==1.0)
        levelRdc = 0.1;
    else
        levelRdc =1.0;

    updateButtons();

}

void cls3260MeterMode::on_txtDescription_textChanged(const QString &arg1)
{
    strDescription = arg1;
}


void cls3260MeterMode::on_grpMinor_clicked(bool checked)
{
    enableMinor= checked;
    updateButtons();
}

void cls3260MeterMode::on_cmbBiasChioce_currentIndexChanged(int index)
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

void cls3260MeterMode::on_cmbBiasChioce_currentIndexChanged(const QString &arg1)
{
    this->biasType = arg1;
}

void cls3260MeterMode::on_cmbAddBiasSpeed_currentIndexChanged(const QString &arg1)
{
    this->biasSpeed= arg1;
}

void cls3260MeterMode::on_btnBiasOnOFF_clicked()
{
    blBiasStatus = ! blBiasStatus;
    updateButtons();
}

void cls3260MeterMode::on_btnBiasValue_clicked()
{
    NumberInput *dlg  = new NumberInput(this);
    dlg->setWindowTitle(tr("设定测试Bias的值"));

    if(dlg->exec()==QDialog::Accepted)
    {
        double value = dlg->getNumber();

        if(biasType==tr("Norm"))
        {
            value = (value<0? 0: value);
            value = (value>1? 1: value);
        }
        else
        {
            value = (value<0? 0: value);
            value = (value> 250? 250:value);
        }

        biasValue = value;
        updateButtons();
    }
}


QString cls3260MeterMode::getMeter()
{
    if(clsRS::getInst().meterSeries=="3260")
        return ":IMP";
    else
        return ":MEAS";
}

void cls3260MeterMode::on_btnALC_clicked()
{
  if(btnALC->text()==tr("ALC OFF"))
      btnALC->setText(tr("ALC ON"));
  else if(btnALC->text() == tr("ALC ON"))
      btnALC->setText(tr("ALC HOLD"));
  else if(btnALC->text() == tr("ALC HOLD"))
      btnALC->setText(tr("ALC OFF"));
  else
  {//do nothing here
  }
  alcStatus = btnALC->text();
}
