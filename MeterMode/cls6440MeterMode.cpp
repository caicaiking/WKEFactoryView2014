#include "cls6440MeterMode.h"
#include <QDebug>
#include "frmSetLimit.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "clsMeterUnit.h"
#include "clsRuningSettings.h"
#include "dlgLevelInputBox.h"
#include "NumberInput.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "dlgWk6440Function1.h"
#include "dlgWk6440Function2.h"
#include "cls6440Calibration.h"
#include <QJsonDocument>
cls6440MeterMode::cls6440MeterMode(WKEMeterMode *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);

    connect(lblMaijor,SIGNAL(Clicked()),this,SLOT(limitMajorClick()));
    connect(lblMinor,SIGNAL(Clicked()),this,SLOT(limitMinorClick()));
    connect(lblRdc,SIGNAL(Clicked()),this,SLOT(limitRdcClick()));
    connect(lblFreqLimit,SIGNAL(Clicked()),this,SLOT(lblFreqLimit_click()));
    connect(lblLLimit,SIGNAL(Clicked()),this,SLOT(lblLLimit_click()));
    connect(lblCLimit,SIGNAL(Clicked()),this,SLOT(lblCLimit_click()));
    connect(lblRLimit,SIGNAL(Clicked()),this,SLOT(lblRLimit_click()));

    strDescription = tr("空白");
    item1="Z";
    item2="θ";
    enableMinor=true;
    level =1;
    levelType="V";
    frequency=10000;
    biasSource = tr("内置");
    blBiasStatus= false;
    range = tr("自动");
    rangeRdc = tr("自动");
    speed = tr("最快");
    equcct = tr("串联");
    alc = tr("关");
    levelRdc = 1;
    majorUnit="";
    minorUnit="";
    rdcUnit="";

    dblStartFreq = 109.09E3;
    dblStopFreq = 133.33E3;
    strResEqucct = tr("串联");
    strResSpeed = tr("最快");
    intDeeps = 12;
    strFreqUnit="k";
    strRUnit="";
    strCUnit="n";
    strLUnit="n";
    strQUnit="";

    blStop = false;

    updateButtons();
}


bool cls6440MeterMode::detectDut(int threshold)
{
    blStop= true;
    bool isEmpty=false;

    QString item;

    if(clsRS::getInst().gpibCommands.testMode=="AC")
    {
        int index = clsRS::getInst().sendCommand(":MEAS:FUNC:MAJOR?",true).toInt();

        switch (index) {
        case 0:
            item="C";
            break;
        case 1:
            item="L";
            break;
        case 2:
            item="X";
            break;
        case 3:
            item="B";

            break;
        case 4:
            item="Z";
            break;
        case 5:
            item="Y";
            break;
        default:
            break;
        }

        if(item!="C")
            clsRS::getInst().sendCommand(QString(":MEAS:FUNC:C"),false);
    }

    while(blStop)
    {


        if(clsRS::getInst().gpibCommands.testMode=="RDC")
        {
            QString retValue = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

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
            QString retValue = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

            double value = retValue.split(",").at(0).toDouble();

            if(qAbs(value)>5E-12)
            {
                if(isEmpty)
                {
                    clsRS::getInst().sendCommand(QString(":MEAS:FUNC:%1").arg(item),false);
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

void cls6440MeterMode::stopDetect()
{
    blStop = false;
    qApp->processEvents();
}

void cls6440MeterMode::setCondition(QString value)
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
                this->biasSource=result["biasSource"].toString();
                this->blBiasStatus = result["biasStatus"].toBool();
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
                this->alc = result["alc"].toString();

                //Res search mode
                this->dblStartFreq = result["dblStartFrequency"].toDouble();
                this->dblStopFreq = result["dblStopFreq"].toDouble();
                this->strResEqucct = result["strResEqucct"].toString();
                this->strResSpeed = result["strResSpeed"].toString();
                this->intDeeps = result["intDeeps"].toInt();
                this->strFreqUnit = result["strFreqUnit"].toString();
                strRUnit = result["strRUnit"].toString();
                strCUnit = result["strCUnit"].toString();
                strLUnit = result["strLUnit"].toString();
                strQUnit = result["strQUnit"].toString();
                lmFreq.setString( result["lmFreq"].toString());
                lmR.setString(result["lmR"].toString());
                lmC.setString(result["lmC"].toString());
                lmL.setString(result["lmL"].toString());
                lmQ.setString(result["lmQ"].toString());
                this->blFreq=result["blFreq"].toBool();
                this->blR = result["blR"].toBool();
                this->blC = result["blC"].toBool();
                this->blL = result["blL"].toBool();
                updateButtons();
            }
        }
    }
}

QString cls6440MeterMode::getConditon()
{
    QVariantMap step;
    step.insert("currentIndex",tabMeter->currentIndex());
    step.insert("frequency",this->frequency);
    step.insert("level",this->level);
    step.insert("leveType",this->levelType);
    step.insert("equcct",this->equcct);
    step.insert("range",this->range);
    step.insert("speed",this->speed);
    step.insert("biasSource",this->biasSource);
    step.insert("biasStatus",this->blBiasStatus);
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
    step.insert("alc", this->alc);

    step.insert("description",this->strDescription);

    //Res search mode
    step.insert("dblStartFrequency",this->dblStartFreq);
    step.insert("dblStopFreq",this->dblStopFreq);
    step.insert("strResEqucct",this->strResEqucct);
    step.insert("strResSpeed",this->strResSpeed);
    step.insert("intDeeps",this->intDeeps);
    step.insert("strFreqUnit",this->strFreqUnit);
    step.insert("strRUnit",this->strRUnit);
    step.insert("strCUnit",this->strCUnit);
    step.insert("strLUnit",this->strLUnit);
    step.insert("strQUnit",this->strQUnit);
    step.insert("lmFreq",this->lmFreq.toString());
    step.insert("lmR",this->lmR.toString());
    step.insert("lmC",this->lmC.toString());
    step.insert("lmL",this->lmL.toString());
    step.insert("lmQ",this->lmQ.toString());
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

void cls6440MeterMode::updateGPIB()
{
    if(tabMeter->currentIndex()==0)
    {
        if(clsRS::getInst().gpibCommands.testMode !="AC")
        {
            clsRS::getInst().sendCommand(":MEAS",false);
            clsRS::getInst().sendCommand(":MEAS:TEST",false);
            clsRS::getInst().sendCommand(":MEAS:TEST:AC",false);
            clsRS::getInst().gpibCommands.testMode ="AC";
        }
        QStringList gpibCmd;
        QString meter=":MEAS:";
        // QString meter2="MEAS:";

        if(equcct==tr("串联"))
            gpibCmd.append(meter+"EQU-CCT "+"SER");
        else
            gpibCmd.append(meter+"EQU-CCT "+"PAR");

        if(alc == tr("关"))
            gpibCmd.append(meter + "ALC "+ "OFF");
        else
             gpibCmd.append(meter + "ALC "+ "ON");



        if(item1 !="Z" && item1 !="Y")
        {
            gpibCmd.append(meter+"FUNC:"+item1 +";" +item2);
        }
        else
        {
            gpibCmd.append(meter+"FUNC:"+ item1 );  //item1 GPIB
        }

        if(range==tr("自动"))
            gpibCmd.append(meter+"RANGE "+"AUTO");
        else
            gpibCmd.append(meter+"RANGE "+range);

        if(speed==tr("最快"))
            gpibCmd.append(meter+"SPEED "+"MAX");
        else if(speed==tr("快速"))
            gpibCmd.append(meter+"SPEED "+"FAST");
        else if(speed==tr("中速"))
            gpibCmd.append(meter+"SPEED "+"MED");
        else if(speed==tr("慢速"))
            gpibCmd.append(meter+"SPEED "+"SLOW");
        else
            gpibCmd.append(meter+"SPEED "+"MAX");

        gpibCmd.append(meter+"FREQ "+QString::number(frequency));

        if(levelType=="V")
        {
            gpibCmd.append(meter+"LEV "+QString::number(level)+"V");
        }
        else
        {
            gpibCmd.append(meter+"LEV "+QString::number(level)+"A");
        }

        if(biasSource==tr("内置"))
        {
            gpibCmd.append(meter+"BIAS "+"VINT");
        }
        else
        {
            gpibCmd.append(meter+"BIAS "+"VEXT");
        }



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
            tmpBiasString=meter+"BIAS "+"OFF";
        }
        else
        {
            tmpBiasString=meter+"BIAS "+"ON";
        }

        if(clsRS::getInst().gpibCommands.biasCommand != tmpBiasString)
        {
            clsRS::getInst().sendCommand(tmpBiasString,false);
            clsRS::getInst().gpibCommands.biasCommand = tmpBiasString;
        }
        emit biasStatus(blBiasStatus);




        QString xx =QString(":MEAS:FREQ?");
        QString retFreq = clsRS::getInst().sendCommand(xx,true);

        this->frequency = retFreq.toDouble();
        updateButtons();
    }

    else if(tabMeter->currentIndex()==1)
    {
        if(clsRS::getInst().gpibCommands.testMode !="RDC")
        {
            clsRS::getInst().sendCommand(":MEAS",false);
            clsRS::getInst().sendCommand(":MEAS:TEST",false);
            clsRS::getInst().sendCommand(":MEAS:TEST:RDC",false);
            clsRS::getInst().gpibCommands.testMode ="RDC";
        }

        QStringList gpibCmd;
        QString meter=":MEAS:";

        if(speed==tr("最快"))
            gpibCmd.append(meter+"SPEED "+"MAX");
        else if(speed==tr("快速"))
            gpibCmd.append(meter+"SPEED "+"FAST");
        else if(speed==tr("中速"))
            gpibCmd.append(meter+"SPEED "+"MED");
        else if(speed==tr("慢速"))
            gpibCmd.append(meter+"SPEED "+"SLOW");
        else
            gpibCmd.append(meter+"SPEED "+"MAX");

        if(range==tr("自动"))
            gpibCmd.append(meter+"RANGE "+"AUTO");
        else
            gpibCmd.append(meter+"RANGE "+range);
        gpibCmd.append(meter+"LEV "+QString::number(levelRdc)+"V");

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
        clsRS::getInst().gpibCommands.biasCommand = ":MEAS:BIAS OFF";
        emit biasStatus(false);
    }
    else //Res mode command
    {
        if(clsRS::getInst().gpibCommands.testMode !="RES")
        {
            clsRS::getInst().sendCommand(":RESO",false);
            clsRS::getInst().gpibCommands.testMode ="RES";
            clsRS::getInst().gpibCommands.gpibTest1.clear();
            clsRS::getInst().gpibCommands.gpibTest2.clear();
        }


        QStringList gpibCmd;
        QString meter=":RESO:";

        gpibCmd.append(meter+"ST "+QString::number(dblStartFreq));
        gpibCmd.append(meter+"SP "+QString::number(dblStopFreq));

        if(strResEqucct==tr("串联"))
            gpibCmd.append(meter+"EQU-CCT "+"SER");
        else
            gpibCmd.append(meter+"EQU-CCT "+"PAR");

        if(speed==tr("最快"))
            gpibCmd.append(meter+"SPEED "+"MAX");
        else if(speed==tr("快速"))
            gpibCmd.append(meter+"SPEED "+"FAST");
        else if(speed==tr("中速"))
            gpibCmd.append(meter+"SPEED "+"MED");
        else if(speed==tr("慢速"))
            gpibCmd.append(meter+"SPEED "+"SLOW");
        else
            gpibCmd.append(meter+"SPEED "+"MAX");

        gpibCmd.append(meter+"EXTRP ON");
        gpibCmd.append(meter+"DEPTH "+QString::number(intDeeps));

        if(clsRS::getInst().gpibCommands.gpibRes.length()!= gpibCmd.length())
        {
            clsRS::getInst().gpibCommands.gpibRes.clear();

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
                if(clsRS::getInst().gpibCommands.gpibRes.at(i)!= gpibCmd.at(i))
                {
                    clsRS::getInst().sendCommand(gpibCmd.at(i),false);
                    UserfulFunctions::sleepMs(50);
                }
            }
        }

        clsRS::getInst().gpibCommands.gpibRes = gpibCmd;


    }
}

bool cls6440MeterMode::getTotalStatus()
{
    bool tmp = true;

    if(tabMeter->currentIndex()==1)
    {
        return lmRdc.comparaValue(dblRdc);
    }
    else if(tabMeter->currentIndex()==0)
    {
        tmp = lmMajor.comparaValue(this->dblItem1);
        if(grpMinor->isChecked())
            tmp = tmp && lmMinor.comparaValue(dblItem2);

        return tmp;
    }

}

int cls6440MeterMode::getCountTestItems()
{
    if(tabMeter->currentIndex()==0)
    {
        return 1+(grpMinor->isChecked()?1:0);
    }
    else if(tabMeter->currentIndex()==1)
    {
        return 1;
    }
    else if(tabMeter->currentIndex()==2)
    {
        int testItemCount=0;
        if(blFreq)
            testItemCount++;
        if(blL)
            testItemCount++;
        if(blR)
            testItemCount++;
        if(blC)
            testItemCount++;
        return testItemCount;
    }
    else
        return 0;
}

QString cls6440MeterMode::getItem(int i)
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
    else if(tabMeter->currentIndex()==2)
    {
        QStringList items;
        if(blFreq)
            items.append("SRF");
        if(blL)
            items.append("L");
        if(blR)
            items.append("R");
        if(blC)
            items.append("C");

        return items.at(i);
    }
    return "";
}

QString cls6440MeterMode::getSuffix(int i)
{
    if(tabMeter->currentIndex()==1)
    {
        return rdcUnit;
    }
    else if(tabMeter->currentIndex()==0)
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
    else if(tabMeter->currentIndex()==2)
    {
        QStringList items;
        if(blFreq)
            items.append(strFreqUnit);
        if(blL)
            items.append(strLUnit);
        if(blR)
            items.append(strRUnit);
        if(blC)
            items.append(strCUnit);
        return items.at(i);
    }
    return "";
}

QString cls6440MeterMode::getFreq()
{
    return this->btnFrequency->text();
}

QString cls6440MeterMode::getEqucct()
{
    return this->btnEqucct->text();
}

QString cls6440MeterMode::getLevel()
{
    return this->btnLevel->text();
}

void cls6440MeterMode::setItemValue(SweepType t, double value)
{
     switch (t) {
    case Frequency:
        this->frequency = value;
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


double cls6440MeterMode::getResult(int i)
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
    else if(tabMeter->currentIndex()==1)
        return dblRdc;
    else if(tabMeter->currentIndex()==2)
    {
        QList<double> tmp;
        if(blFreq)
            tmp.append(dblFreqRes);
        if(blL)
            tmp.append(dblLRes);
        if(blR)
            tmp.append(dblRRes);
        if(blC)
            tmp.append(dblCRes);
        return tmp.at(i);

    }


}

clsMeterLimit cls6440MeterMode::getLimit(int i)
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
    else if(tabMeter->currentIndex()==2)
    {
        QList<clsMeterLimit> tmp;
        if(blFreq)
            tmp.append(lmFreq);
        if(blL)
            tmp.append(lmL);
        if(blR)
            tmp.append(lmR);
        if(blC)
            tmp.append(lmC);

        return tmp.at(i);
    }
}

void cls6440MeterMode::calibration()
{
    cls6440Calibration *dlg = new cls6440Calibration(this);
    dlg->exec();
}

QString cls6440MeterMode::getBrief()
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
        tmp.append(btnBiasSource->text()+",");
        tmp.append(btnBiasStatus->text());
    }
    else if(tabMeter->currentIndex()==1)
    {
        tmp.append("Rdc");
        tmp.append(",");
        tmp.append(btnRdcLevel->text()+",");
        tmp.append(btnRdcSpeed->text()+",");
        tmp.append(btnRdcRange->text());
    }
    else
    {
        tmp.append("Res");
        tmp.append(",ST:");
        tmp.append(btnStart->text()+",SP:");
        tmp.append(btnStop->text()+",Type");
        tmp.append(btnResEqucct->text());
    }

    return tmp;
}

QString cls6440MeterMode::getMeterSeries()
{
    return clsRS::getInst().meterSeries;
}

QString cls6440MeterMode::getDescription()
{
    return this->strDescription;
}

void cls6440MeterMode::turnOffBias()
{
    if(clsRS::getInst().gpibCommands.testMode=="RDC")
    {
        emit biasStatus(false);
        return;
    }
    else
    {
        if(clsRS::getInst().gpibCommands.biasCommand==":MEAS:BIAS OFF")
        {
            emit biasStatus(false);
        }
        else
        {

            clsRS::getInst().sendCommand(":MEAS:BIAS OFF",false);
            clsRS::getInst().gpibCommands.biasCommand = ":MEAS:BIAS OFF";
            emit biasStatus(false);
        }
    }
}

void cls6440MeterMode::singleTrig()
{
    QString trigCmd;
    if(tabMeter->currentIndex()==0 || tabMeter->currentIndex()==1)
        trigCmd=":MEAS:TRIG";
    else if(tabMeter->currentIndex()==2)
        trigCmd =":RESO:TRIG";

    QString strRes = clsRS::getInst().sendCommand(trigCmd,true);

    strRes+=",,,,,";

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
    }
    else if(tabMeter->currentIndex()==1)
    {
        dblRdc = lsRes.at(0).toDouble();
        QString tmp = getItemShow("RDC",dblRdc,lmRdc,rdcUnit);
        emit signalTestResult(tmp);
    }
    else if(tabMeter->currentIndex()==2)
    {
        dblFreqRes = lsRes.at(0).toDouble();
        if(strResEqucct==tr("串联"))
            dblLRes = lsRes.at(1).toDouble();
        else
            dblCRes = lsRes.at(1).toDouble();
        dblRRes=lsRes.at(2).toDouble();

        QString tmp;
        for(int i=0; i< getCountTestItems(); i++)
        {
            clsMeterLimit st = getLimit(i);
            tmp += getItemShow(getItem(i),getResult(i),st,getResUnit(i));
            if(i< getCountTestItems()-1)
                tmp+="|";
        }
         emit signalTestResult(tmp);
    }


}

QString cls6440MeterMode::getResUnit(int i)
{
    QStringList tmp;
    if(blFreq)
        tmp.append(strFreqUnit);
    if(blL)
        tmp.append(strLUnit);
    if(blR)
        tmp.append(strRUnit);
    if(blC)
        tmp.append(strCUnit);

    return tmp.at(i);
}

QString cls6440MeterMode::getItemShow(const QString &item, const double &value , clsMeterLimit &limit, const QString &/*suffix*/)
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

void cls6440MeterMode::repetiveTrig()
{
    QString trigCmd;
    if(tabMeter->currentIndex()==0 || tabMeter->currentIndex()==1)
        trigCmd=":MEAS:TRIG";
    else if(tabMeter->currentIndex()==2)
        trigCmd =":RESO:TRIG";

    QString strRes = clsRS::getInst().sendCommand(trigCmd,true);

    strRes+=",,,,";
    QStringList lsRes = strRes.split(",");
    if(tabMeter->currentIndex()==0)
    {
        dblItem1 = lsRes.at(0).toDouble();
        dblItem2 = lsRes.at(1).toDouble();
    }
    else  if(tabMeter->currentIndex()==1)
    {
        dblRdc = lsRes.at(0).toDouble();
    }
    else if(tabMeter->currentIndex()==2)
    {
        dblFreqRes = lsRes.at(0).toDouble();
        if(strResEqucct==tr("串联"))
            dblLRes = lsRes.at(1).toDouble();
        else
            dblCRes = lsRes.at(1).toDouble();
        dblRRes=lsRes.at(2).toDouble();
    }

}


void cls6440MeterMode::limitMajorClick()
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

void cls6440MeterMode::limitMinorClick()
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

void cls6440MeterMode::limitRdcClick()
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

void cls6440MeterMode::updateButtons()
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

    btnBiasSource->setText(biasSource);
    btnBiasStatus->setText(blBiasStatus?tr("开"):tr("关"));

    btnRange->setText(range);
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

    this->txtDescription->setText(this->strDescription);

    //Res Search
    dt.setData(dblStartFreq);
    btnStart->setText(dt.formateToString(6)+"Hz");
    dt.setData(dblStopFreq);
    btnStop->setText(dt.formateToString(6)+"Hz");
    btnResEqucct->setText(strResEqucct);

    if(strResEqucct==tr("串联"))
    {
        grpC->setVisible(false);
        grpL->setVisible(true);
        blC=false;
    }
    else
    {
        grpL->setVisible(false);
        grpC->setVisible(true);
        blL = false;
    }

    grpFreq->setChecked(blFreq);
    grpL->setChecked(blL);
    grpR->setChecked(blR);
    grpC->setChecked(blC);


    btnResSpeed->setText(strResSpeed);
    btnDepth->setText(QString::number(intDeeps));

    lblFreqLimit->setText(lmFreq.showLimits("Hz"));
    lblRLimit->setText(lmR.showLimits(UserfulFunctions::getSuffix("R")));
    lblCLimit->setText(lmR.showLimits(UserfulFunctions::getSuffix("C")));
    lblLLimit->setText(lmR.showLimits(UserfulFunctions::getSuffix("L")));

    btnFreqUnit->setText(strFreqUnit+"Hz");
    btnLUnit->setText(strLUnit+UserfulFunctions::getSuffix("L"));
    btnCUnit->setText(strCUnit+UserfulFunctions::getSuffix("C"));
    btnRUnit->setText(strRUnit+UserfulFunctions::getSuffix("R"));
    btnALC->setText(alc);


}


void cls6440MeterMode::on_btnMajorUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(this->item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        majorUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnMinorUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(this->item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        minorUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnRdcUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("RDC");
    if(dlg->exec()==QDialog::Accepted)
    {
        rdcUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnFrequency_clicked()
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

double cls6440MeterMode::getMaxFreq()
{
    if(clsRS::getInst().instrumentModel=="6430")
        return 500000;
    else
        return 3000000;
}

void cls6440MeterMode::on_btnLevel_clicked()
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

double cls6440MeterMode::getOkLevel(double value, QString unit)
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

void cls6440MeterMode::on_btnEqucct_clicked()
{
    if(this->equcct ==tr("串联"))
        this->equcct = tr("并联");
    else
        this->equcct =tr("串联") ;
    updateButtons();

}

void cls6440MeterMode::on_btnRange_clicked()
{
    wk6500Range *dlg=new wk6500Range;
    dlg->setWindowTitle(tr("设置档位"));

    if(dlg->exec()==QDialog::Accepted)
    {
        range=dlg->getRange();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnSpeed_clicked()
{
    dlgSpeed *dlg=new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        speed=dlg->getSpeed();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnBiasSource_clicked()
{
    if(this->biasSource ==tr("内置"))
        this->biasSource = tr("外置");
    else
        this->biasSource = tr("内置");

    updateButtons();
}

void cls6440MeterMode::on_btnBiasStatus_clicked()
{

    blBiasStatus = !blBiasStatus;
    updateButtons();
}

void cls6440MeterMode::on_btnItem1_clicked()
{
    dlgWk6440Function1 *dlg=new dlgWk6440Function1();
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

void cls6440MeterMode::on_btnItem2_clicked()
{
    if(this->btnItem1->text()=="Z"|| btnItem1->text()=="Y")
    {
        this->btnItem2->setText("θ");
        this->item2="θ";
        updateButtons();
        return;
    }

    dlgWk6440Function2 *dlg = new dlgWk6440Function2;
    dlg->setWindowTitle(tr("设置测试项目2"));

    if(dlg->exec()==QDialog::Accepted)
    {
        item2 = dlg->getItem();
        updateButtons();
    }
}


void cls6440MeterMode::on_btnRdcRange_clicked()
{
    wk6500Range *dlg=new wk6500Range;
    dlg->setWindowTitle(tr("设置档位"));

    if(dlg->exec()==QDialog::Accepted)
    {
        rangeRdc=dlg->getRange();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnRdcSpeed_clicked()
{
    dlgSpeed *dlg=new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        speed=dlg->getSpeed();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnRdcLevel_clicked()
{
    if(levelRdc==1.0)
        levelRdc = 0.1;
    else
        levelRdc =1.0;

    updateButtons();

}

void cls6440MeterMode::on_txtDescription_textChanged(const QString &arg1)
{
    strDescription = arg1;
}


void cls6440MeterMode::on_grpMinor_clicked(bool checked)
{
    enableMinor= checked;
    updateButtons();
}

void cls6440MeterMode::on_btnStart_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入开始频率"));

    doubleType dt;
    dt.setData(this->dblStartFreq);

    double tmp;
    QString suffix;

    dt.getDataAndUnit(tmp,suffix);
    dlg->setValueAndSuffix(tmp,suffix);


    if(dlg->exec()==QDialog::Accepted)
    {
        tmp = dlg->getNumber();

        tmp = tmp>getMaxFreq()? getMaxFreq(): tmp;
        tmp = tmp<20.0? 20: tmp;

        this->dblStartFreq = tmp;
        updateButtons();
    }

}

void cls6440MeterMode::on_btnStop_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入终止频率"));

    doubleType dt;
    dt.setData(this->dblStopFreq);

    double tmp;
    QString suffix;

    dt.getDataAndUnit(tmp,suffix);
    dlg->setValueAndSuffix(tmp,suffix);


    if(dlg->exec()==QDialog::Accepted)
    {
        tmp = dlg->getNumber();

        tmp = tmp>getMaxFreq()? getMaxFreq(): tmp;
        tmp = tmp<20.0? 20: tmp;

        this->dblStopFreq = tmp;
        updateButtons();
    }

}

void cls6440MeterMode::on_btnResEqucct_clicked()
{
    if(this->strResEqucct ==tr("串联"))
        this->strResEqucct = tr("并联");
    else
        this->strResEqucct =tr("串联") ;
    updateButtons();
}

void cls6440MeterMode::on_btnDepth_clicked()
{
    NumberInput *dlg = new NumberInput;
    dlg->setWindowTitle(tr("设定扫描深度(0-12)"));
    dlg->setValueAndSuffix(intDeeps,"");

    if(dlg->exec()==QDialog::Accepted)
    {
        intDeeps = (int)dlg->getNumber();
        intDeeps =(intDeeps<0? 0:intDeeps);
        intDeeps =(intDeeps>12?12:intDeeps);
        updateButtons();
    }
}

void cls6440MeterMode::on_btnResSpeed_clicked()
{
    dlgSpeed *dlg=new dlgSpeed(this);
    dlg->setWindowTitle(tr("扫描速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        strResSpeed=dlg->getSpeed();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnFreqUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("Frequency");
    if(dlg->exec()==QDialog::Accepted)
    {
        strFreqUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnRUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("R");
    if(dlg->exec()==QDialog::Accepted)
    {
        strRUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnCUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("C");
    if(dlg->exec()==QDialog::Accepted)
    {
        strCUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::on_btnLUnit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem("L");
    if(dlg->exec()==QDialog::Accepted)
    {
        strLUnit = dlg->getSuffix();
        updateButtons();
    }
}

void cls6440MeterMode::lblFreqLimit_click()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(lmFreq);
    dlg->setWindowTitle(tr("设置上下限"));
    dlg->setItem("Frequency");
    if(dlg->exec()==QDialog::Accepted)
    {
        lmFreq = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6440MeterMode::lblLLimit_click()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(lmL);
    dlg->setWindowTitle(tr("设置上下限"));
    dlg->setItem("L");
    if(dlg->exec()==QDialog::Accepted)
    {
        lmL = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6440MeterMode::lblCLimit_click()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(lmC);
    dlg->setWindowTitle(tr("设置上下限"));
    dlg->setItem("C");
    if(dlg->exec()==QDialog::Accepted)
    {
        lmC = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6440MeterMode::lblRLimit_click()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(lmR);
    dlg->setWindowTitle(tr("设置上下限"));
    dlg->setItem("R");
    if(dlg->exec()==QDialog::Accepted)
    {
        lmR = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls6440MeterMode::on_grpFreq_toggled(bool arg1)
{
    blFreq = arg1;
    updateButtons();
}

void cls6440MeterMode::on_grpR_toggled(bool arg1)
{
    blR=arg1;
    updateButtons();
}

void cls6440MeterMode::on_grpC_toggled(bool arg1)
{
    blC=arg1;
    updateButtons();
}

void cls6440MeterMode::on_grpL_toggled(bool arg1)
{
    blL = arg1;
    updateButtons();
}

void cls6440MeterMode::on_btnALC_clicked()
{
    if(btnALC->text() == tr("关"))
        btnALC->setText(tr("开"));
    else
        btnALC->setText(tr("关"));

    alc = btnALC->text();
    updateButtons();
}
