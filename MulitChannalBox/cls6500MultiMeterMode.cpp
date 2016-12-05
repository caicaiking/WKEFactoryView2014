#include "cls6500MultiMeterMode.h"
#include <QObject>
#include "doubleType.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "frmSetLimit.h"
#include "NumberInput.h"
#include "dlgLevelInputBox.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "dlgFunction.h"
#include "clsMeterUnit.h"
#include <QJsonDocument>
#include <QVariantMap>
#include "clsCalibrationDbOp.h"
#include "clsDataProcess.h"
#include "wk6500Calibration.h"
#include "clsSingleTrig.h"
cls6500MultiMeterMode::cls6500MultiMeterMode()
{
    test1Freq=10000.0;
    test1LevelV=1.0;
    test1LevelA=0.01;
    test1LevelType="V";
    test1Equcct=QString(QObject::tr("串联"));

    test2IsOn=false;
    test2Freq=10000;
    test2LevelA=1.0;
    test2LevelV=1.0;
    test2LevelType="V";
    test2Equcct=QObject::tr("串联");

    range=QObject::tr("自动");
    speed=QObject::tr("最快");

    isTest1Item2On= true;
    isTest2Item2On= false;

    test1Item1= "C";
    test1Item1Unit=QObject::tr("OFF");

    test1Item2= "D";
    test1Item2Unit=QObject::tr("OFF");

    test2Item1= "L";
    test2Item1Unit= QObject::tr("OFF");
    test2Item2="R";
    test2Item2Unit=QObject::tr("OFF");
}

QString cls6500MultiMeterMode::getConditon()
{
    QVariantMap  tmp;
    tmp.insert("range",this->range);
    tmp.insert("speed",this->speed);

    tmp.insert("test1Freq",this->test1Freq);
    tmp.insert("test1LevelA",test1LevelA);
    tmp.insert("test1LevelType",this->test1LevelType);
    tmp.insert("test1LevelV",this->test1LevelV);
    tmp.insert("test1Equcct",this->test1Equcct);

    tmp.insert("test1Item1",test1Item1);
    tmp.insert("test1Item1Unit",test1Item1Unit);
    tmp.insert("test1Item1Limit",test1Item1Limit.toString());

    tmp.insert("isTest1Item2On",this->isTest1Item2On);
    tmp.insert("test1Item2",this->test1Item2);
    tmp.insert("test1Item2Unit",test1Item2Unit);
    tmp.insert("test1Item2Limit",test1Item2Limit.toString());

    tmp.insert("test2IsOn",test2IsOn);
    tmp.insert("test2Freq",test2Freq);
    tmp.insert("test2LevelA",test2LevelA);
    tmp.insert("test2LevelType",test2LevelType);
    tmp.insert("test2LevelV",test2LevelV);
    tmp.insert("test2Equcct",test2Equcct);

    tmp.insert("test2Item1",test2Item1);
    tmp.insert("test2Item1Unit",test2Item1Unit);
    tmp.insert("test2Item1Limit",test2Item1Limit.toString());
    tmp.insert("isTest2Item2On",isTest2Item2On);
    tmp.insert("test2Item2",test2Item2);
    tmp.insert("test2Item2Unit",test2Item2Unit);
    tmp.insert("test2Item2Limit",test2Item2Limit.toString());

    QJsonDocument json = QJsonDocument::fromVariant(tmp);
    if(!json.isNull())
        return json.toJson();
    return "";
}

void cls6500MultiMeterMode::calibration()
{
    wk6500Calibration *dlg = new wk6500Calibration(wk6500Calibration::GetMaxFrequency());
    dlg->exec();
    dlg->close();
}

void cls6500MultiMeterMode::turnOnScreen(bool value)
{
    if(value)
        clsRS::getInst().sendCommand(":METER:FAST-GPIB ON");
    else
        clsRS::getInst().sendCommand(":METER:FAST-GPIB OFF");
}

void cls6500MultiMeterMode::setChannel(int value)
{
    this->channel = value;
}

void cls6500MultiMeterMode::setUseLoad(bool value)
{
    this->useLoad = value;
}

void cls6500MultiMeterMode::set10KHz()
{
    this->setFrequcy(10000);
}

void cls6500MultiMeterMode::setConditionForCalibration(int i)
{
    updateGpib();

    if(i==0)
        setLevel(test1LevelV,test1LevelA,test1LevelType);
    else
        setLevel(test2LevelV,test2LevelA,test2LevelType);
    clsRS::getInst().sendCommand(":METER:EQU-CCT SER");

    // clsRS::getInst().sendCommand(":MEAS:LEV 1.0V"); //设置测试条件为1V，仪器有更好的精准度
    //clsRS::getInst().sendCommand(":FAST-GPIB ON");
    // clsRS::getInst().sendCommand(":METER:FREQ 10000");
    clsRS::getInst().sendCommand(":METER:SPEED SLOW"); //设置测试速度慢速
    clsRS::getInst().gpibCommands.gpibTest1.clear(); //清除内存中GPIB指令

}



QList<double> cls6500MultiMeterMode::getOriginZA()
{
    QString strRes = clsRS::getInst().sendCommand(":METER:TRIG",true);
    return UserfulFunctions::resultPro(strRes+",,,,");
}

void cls6500MultiMeterMode::setFreqencyForCal(int value)
{
    if(value ==0)
        setFrequcy(test1Freq);
    else
        setFrequcy(test2Freq);
}

void cls6500MultiMeterMode::setFreqencyForCal(double value)
{
    this->setFrequcy(value);
}

double cls6500MultiMeterMode::getFreqency(int value)
{
    if(value ==0 )
        return this->test1Freq;
    else
        return this->test2Freq;
}

void cls6500MultiMeterMode::setCondition(QString value)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(value.toUtf8(),&error);

    if(error.error != QJsonParseError::NoError)
        return;

    if((jsonDocument.isNull()|| jsonDocument.isEmpty()))
        return;

    if(!jsonDocument.isObject())
        return;

    QVariantMap result = jsonDocument.toVariant().toMap();

    this->range = result["range"].toString();
    this->speed = result["speed"].toString();

    this->test1Freq = result["test1Freq"].toDouble();
    this->test1LevelA = result["test1LevelA"].toDouble();
    this->test1LevelType = result["test1LevelType"].toString();
    this->test1LevelV = result["test1LevelV"].toDouble();
    this->test1Equcct = result["test1Equcct"].toString();

    this->test1Item1 = result["test1Item1"].toString();
    this->test1Item1Unit = result["test1Item1Unit"].toString();
    this->test1Item1Limit.setString(result["test1Item1Limit"].toString());

    this->isTest1Item2On  = result["isTest1Item2On"].toBool();
    this->test1Item2  = result["test1Item2"].toString();
    this->test1Item2Unit = result["test1Item2Unit"].toString();
    this->test1Item2Limit.setString(result["test1Item2Limit"].toString());

    this->test2IsOn  = result["test2IsOn"].toBool();
    this->test2Freq = result["test2Freq"].toDouble();
    this->test2LevelA  = result["test2LevelA"].toDouble();
    this->test2LevelType = result["test2LevelType"].toString();
    this->test2LevelV = result["test2LevelV"].toDouble();
    this->test2Equcct =result["test2Equcct"].toString();

    this->test2Item1 = result["test2Item1"].toString();
    this->test2Item1Unit = result["test2Item1Unit"].toString();
    this->test2Item2Limit.setString(result["test2Item2Limit"].toString());

    this->isTest2Item2On = result["isTest2Item2On"].toBool();
    this->test2Item2 =result["test2Item2"].toString();
    this->test2Item2Unit =result["test2Item2Unit"].toString();
    this->test2Item2Limit.setString(result["test2Item2Limit"].toString());
}

bool cls6500MultiMeterMode::getStatus(int value)
{
    clsMeterLimit limit = getLimit(value);
    double res = getRes(value);

    return limit.comparaValue(res);
}

bool cls6500MultiMeterMode::getTotleStatus()
{
    bool value = true;
    for(int i=0; i< getTotleItemCount(); i++)
    {
        value = value && getStatus(i);
    }

    return value;
}

clsMeterLimit cls6500MultiMeterMode::getLimit(int value)
{
    QList<clsMeterLimit> tmp;
    tmp.append(test1Item1Limit);
    if(isTest1Item2On)
        tmp.append(test1Item2Limit);
    if(test2IsOn)
    {
        tmp.append(test2Item1Limit);

        if(isTest2Item2On)
            tmp.append(test2Item2Limit);
    }

    return tmp.at(value);
}

double cls6500MultiMeterMode::getRes(int value)
{
    QList<double> tmp;

    tmp.append(test1Res1);
    if(isTest1Item2On)
        tmp.append(test1Res2);
    if(test2IsOn)
    {
        tmp.append(test2Res1);

        if(isTest2Item2On)
            tmp.append(test2Res2);
    }

    return tmp.at(value);
}

QString cls6500MultiMeterMode::getUnit(int value)
{
    QStringList tmp;
    tmp.append(test1Item1Unit);
    if(isTest1Item2On)
        tmp.append(test1Item2Unit);
    if(test2IsOn)
    {
        tmp.append(test2Item1Unit);

        if(isTest2Item2On)
            tmp.append(test2Item2Unit);
    }

    return tmp.at(value);
}

QString cls6500MultiMeterMode::getItem(int value)
{
    QStringList tmp;
    tmp.append(test1Item1);
    if(isTest1Item2On)
        tmp.append(test1Item2);
    if(test2IsOn)
    {
        tmp.append(test2Item1);

        if(isTest2Item2On)
            tmp.append(test2Item2);
    }

    return tmp.at(value);
}

int cls6500MultiMeterMode::getTotleItemCount()
{
    int count =1;
    if(isTest1Item2On)
        count ++;
    if(test2IsOn)
    {
        count++;
        if(isTest2Item2On)
            count ++;
    }

    return count;
}

void cls6500MultiMeterMode::updateGpib()
{
    QStringList cmmds = this->convertGpib();

    if(cmmds != clsRS::getInst().gpibCommands.gpibTest1)
    {
        for(int i=0; i<cmmds.length();i++)
        {
            clsRS::getInst().sendCommand(cmmds.at(i));
        }
        clsRS::getInst().gpibCommands.gpibTest1=cmmds;
    }
}

void cls6500MultiMeterMode::setFrequcy(double value)
{
    double queryFreq = clsRS::getInst().sendCommand(":METER:FREQ?",true).toDouble();
    if(queryFreq != value)
    {
        clsRS::getInst().sendCommand(":METER:FREQ "+ QString::number(value,'f',0));
    }
}

void cls6500MultiMeterMode::setLevel(double levelV, double levelA, QString type)
{


    QString strLevel;

    QString driveType = clsRS::getInst().sendCommand(":METER:DRIVE?",true);
    driveType = (driveType=="0"? "V":"A");

    double driveValue = clsRS::getInst().sendCommand(":METER:LEVEL?",true).toDouble();

    QString drive = QString::number(driveValue,'f',6)+ driveType;


    if(type=="V")
        strLevel = QString::number(levelV,'f',6)+"V";
    else
        strLevel = QString::number(levelA,'f',6)+"A";

    if(drive != strLevel)
        clsRS::getInst().sendCommand(":METER:LEVEL "+strLevel);
}

void cls6500MultiMeterMode::trig()
{
    updateGpib();

    //    QString fastGpib = clsRS::getInst().sendCommand(":FAST-GPIB?",true);
    //    if(fastGpib=="0")
    //        clsRS::getInst().sendCommand(":FAST-GPIB ON");
    //For test1
    setFrequcy(this->test1Freq);
    setLevel(test1LevelV,test1LevelA,test1LevelType);

    QList<double> resValue =getOriginZA();
    if(resValue.length()<2)
        return;

    clsSingleTrig d;
    d.setZm(resValue.at(0));
    d.setAm(resValue.at(1));
    d.setFrequency(this->test1Freq);
    d.setChannel(this->channel);

    if(this->useLoad)
        d.doLoadCalibration();
    else
        d.doRCCalibration();


    test1Res1 = d.getItem(test1Item1,test1Equcct);
    test1Res2 = d.getItem(test1Item2,test1Equcct);



    //For test2
    if(!test2IsOn)
        return;
    //如果测试的电平和频率
    if((test1Freq == test2Freq) && (test1LevelType==test2LevelType))
    {
        if(test1LevelType=="V")
        {
            if(test1LevelV == test2LevelV)
            {
                test2Res1 = d.getItem(test2Item1,test2Equcct);
                test2Res2 = d.getItem(test2Item2,test2Equcct);
                return;
            }
            else
            {
                if(test1LevelA == test2LevelA)
                {
                    test2Res1 = d.getItem(test2Item1,test2Equcct);
                    test2Res2 = d.getItem(test2Item2,test2Equcct);
                    return;
                }
            }
        }
    }

    UserfulFunctions::sleepMs(5);
    setFrequcy(this->test2Freq);
    setLevel(test2LevelV,test2LevelA,test2LevelType);

    QList<double> resValue2 =getOriginZA();
    if(resValue2.length()<2)
        return;

    clsSingleTrig d2;
    d2.setZm(resValue2.at(0));
    d2.setAm(resValue2.at(1));
    d2.setFrequency(this->test2Freq);
    d2.setChannel(this->channel);
    if(this->useLoad)
        d2.doLoadCalibration();
    else
        d2.doRCCalibration();

    test2Res1 =d2.getItem(test2Item1,test2Equcct);
    test2Res2 = d2.getItem(test2Item2,test2Equcct);
}

QString cls6500MultiMeterMode::getSpeed()
{
    return speed;
}

void cls6500MultiMeterMode::setSpeed()
{
    dlgSpeed *dlg = new dlgSpeed();
    dlg->setWindowTitle(QObject::tr("设置速度"));
    if(dlg->exec()==QDialog::Accepted)
    {
        this->speed = dlg->getSpeed();
    }
}

QString cls6500MultiMeterMode::getRange()
{
    return range;
}

void cls6500MultiMeterMode::setRange()
{
    wk6500Range *dlg = new wk6500Range();
    dlg->setWindowTitle(QObject::tr("设置档位"));

    if(dlg->exec() == QDialog::Accepted)
    {
        this->range = dlg->getRange();
    }
}

QString cls6500MultiMeterMode::getTest2Item2Limit()
{

    return test2Item2Limit.showLimits(UserfulFunctions::getSuffix(test2Item2));
}

QString cls6500MultiMeterMode::getTest2Item2Unit()
{
    if(test2Item2Unit != QObject::tr("OFF"))
        return test2Item2Unit + UserfulFunctions::getSuffix(test2Item2);
    else
        return QObject::tr("OFF");
}

QString cls6500MultiMeterMode::getTest2Item2()
{
    return test2Item2;
}

void cls6500MultiMeterMode::setTest2Item2Limit()
{
    frmSetLimit *dlg  = new frmSetLimit();
    dlg->setLimits(this->test2Item2Limit);
    dlg->setItem(this->test2Item2);

    if(dlg->exec() == QDialog::Accepted)
    {
        this->test2Item2Limit = dlg->getMeterLimit();
    }

}

void cls6500MultiMeterMode::setTest2Item2Unit()
{
    clsMeterUnit * dlg = new clsMeterUnit();
    dlg->setOFFEnable(true);
    dlg->setWindowTitle(QObject::tr("设置单位"));
    dlg->setItem(test2Item2);

    if(dlg->exec() == QDialog::Accepted)
    {
        test2Item2Unit = dlg->getSuffix();
    }

}

void cls6500MultiMeterMode::setTest2Item2()
{
    dlgFunction * dlg = new dlgFunction();
    dlg->setWindowTitle(QObject::tr("设置测试参数"));
    if(dlg->exec() == QDialog::Accepted)
    {
        this->test2Item2 = dlg->getItem();
    }
}

QString cls6500MultiMeterMode::getTest2Item1Limit()
{
    return test2Item1Limit.showLimits(UserfulFunctions::getSuffix(test2Item1));
}

QString cls6500MultiMeterMode::getTest2Item1Unit()
{
    if(test2Item1Unit != QObject::tr("OFF"))
        return test2Item1Unit + UserfulFunctions::getSuffix(test2Item1);
    else
        return QObject::tr("OFF");
}

QString cls6500MultiMeterMode::getTest2Item1()
{
    return test2Item1;
}

void cls6500MultiMeterMode::setTest2Item1Limit()
{
    frmSetLimit *dlg  = new frmSetLimit();
    dlg->setLimits(this->test2Item1Limit);
    dlg->setItem(this->test2Item1);

    if(dlg->exec() == QDialog::Accepted)
    {
        this->test2Item1Limit = dlg->getMeterLimit();
    }
}

void cls6500MultiMeterMode::setTest2Item1Unit()
{
    clsMeterUnit * dlg = new clsMeterUnit();
    dlg->setOFFEnable(true);
    dlg->setWindowTitle(QObject::tr("设置单位"));
    dlg->setItem(test2Item1);

    if(dlg->exec() == QDialog::Accepted)
    {
        test2Item1Unit = dlg->getSuffix();
    }

}

void cls6500MultiMeterMode::setTest2Item1()
{
    dlgFunction * dlg = new dlgFunction();
    dlg->setWindowTitle(QObject::tr("设置测试参数"));
    if(dlg->exec() == QDialog::Accepted)
    {
        this->test2Item1 = dlg->getItem();
    }
}

bool cls6500MultiMeterMode::getTest2Item2Enable()
{
    return isTest2Item2On;
}

void cls6500MultiMeterMode::setTest2Item2Enable(bool value)
{
    isTest2Item2On = value;
}

QString cls6500MultiMeterMode::getTest2Equcct()
{
    return test2Equcct;
}

void cls6500MultiMeterMode::setTest2Equcct()
{
    this->test2Equcct = (test2Equcct== QObject::tr("串联")?QObject::tr("并联"):QObject::tr("串联"));
}

QString cls6500MultiMeterMode::getTest2Level()
{
    doubleType dt;
    if(test2LevelType =="V")
    {
        test2LevelV = checkLevel(test2LevelV, test2Freq,test2LevelType);
        dt.setData(test2LevelV);
        return dt.formateToString()+"V";
    }
    else
    {
        test2LevelA = checkLevel(test2LevelA,test2Freq, test2LevelType);
        dt.setData(test2LevelA);
        return dt.formateToString()+"A";
    }
}

void cls6500MultiMeterMode::setTest2Level()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox;
    // dlg->setAVisible(false);
    dlg->setWindowTitle(QObject::tr("设置电平"));

    doubleType dt;

    if(test2LevelType=="V")
        dt.setData(test2LevelV,"");
    else
        dt.setData(test2LevelA,"");

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
            test2LevelV = dt.Data();
        }
        else
        {
            test2LevelA = dt.Data();
        }

        test2LevelType = unit;
    }
}

QString cls6500MultiMeterMode::getTest2Freq()
{
    doubleType dt;

    test2Freq = checkFreq(test2Freq);

    dt.setData(test2Freq);
    return dt.formateToString()+"Hz";
}

void cls6500MultiMeterMode::setTest2Freq()
{
    NumberInput * dlg = new NumberInput();
    dlg->setWindowTitle(QObject::tr("输入频率"));
    dlg->setValueAndSuffix(this->test2Freq,"");

    if(dlg->exec()== QDialog::Accepted)
    {
        this->test2Freq = dlg->getNumber();
    }

}

bool cls6500MultiMeterMode::getTest2On()
{
    return test2IsOn;
}

void cls6500MultiMeterMode::setTest2On(bool value)
{
    test2IsOn = value;
}

QString cls6500MultiMeterMode::getTest1Item2Limit()
{
    return test1Item2Limit.showLimits(UserfulFunctions::getSuffix(test1Item2));
}

QString cls6500MultiMeterMode::getTest1Item2Unit()
{
    if(test1Item2Unit != QObject::tr("OFF"))
        return test1Item2Unit + UserfulFunctions::getSuffix(test1Item2);
    else
        return QObject::tr("OFF");

}

QString cls6500MultiMeterMode::getTest1Item2()
{
    return test1Item2;
}

void cls6500MultiMeterMode::setTest1Item2Limit()
{
    frmSetLimit *dlg  = new frmSetLimit();
    dlg->setLimits(this->test1Item2Limit);
    dlg->setItem(this->test1Item2);

    if(dlg->exec() == QDialog::Accepted)
    {
        this->test1Item2Limit = dlg->getMeterLimit();
    }
}

void cls6500MultiMeterMode::setTest1Item2Unit()
{
    clsMeterUnit * dlg = new clsMeterUnit();
    dlg->setOFFEnable(true);
    dlg->setWindowTitle(QObject::tr("设置单位"));
    dlg->setItem(test1Item2);

    if(dlg->exec() == QDialog::Accepted)
    {
        test1Item2Unit = dlg->getSuffix();
    }
}

void cls6500MultiMeterMode::setTest1Item2()
{
    dlgFunction * dlg = new dlgFunction();
    dlg->setWindowTitle(QObject::tr("设置测试参数"));
    if(dlg->exec() == QDialog::Accepted)
    {
        this->test1Item2 = dlg->getItem();
    }
}

bool cls6500MultiMeterMode::getTest1Item2Enable()
{
    return isTest1Item2On;
}

void cls6500MultiMeterMode::setTest1Item2Enable(bool value)
{
    isTest1Item2On = value;
}

QString cls6500MultiMeterMode::getTest1Item1Limit()
{
    return test1Item1Limit.showLimits(UserfulFunctions::getSuffix(test1Item1));
}

void cls6500MultiMeterMode::setTest1Item1Limit()
{
    frmSetLimit *dlg  = new frmSetLimit();
    dlg->setLimits(this->test1Item1Limit);
    dlg->setItem(this->test1Item1);

    if(dlg->exec() == QDialog::Accepted)
    {
        this->test1Item1Limit = dlg->getMeterLimit();
    }
}

QString cls6500MultiMeterMode::getTest1Item1Unit()
{
    if(test1Item1Unit != QObject::tr("OFF"))
        return test1Item1Unit + UserfulFunctions::getSuffix(test1Item1);
    else
        return QObject::tr("OFF");


}

void cls6500MultiMeterMode::setTest1Item1Unit()
{
    clsMeterUnit * dlg = new clsMeterUnit();
    dlg->setOFFEnable(true);
    dlg->setWindowTitle(QObject::tr("设置单位"));
    dlg->setItem(test1Item1);

    if(dlg->exec() == QDialog::Accepted)
    {
        test1Item1Unit = dlg->getSuffix();
    }
}

QString cls6500MultiMeterMode::getTest1Item1()
{
    return test1Item1;
}

void cls6500MultiMeterMode::setTest1Item1()
{
    dlgFunction * dlg = new dlgFunction();
    dlg->setWindowTitle(QObject::tr("设置测试参数"));
    if(dlg->exec() == QDialog::Accepted)
    {
        this->test1Item1 = dlg->getItem();
    }
}

QString cls6500MultiMeterMode::getTest1Equcct()
{
    return test1Equcct;
}

void cls6500MultiMeterMode::setTest1Equcct()
{
    this->test1Equcct = (test1Equcct== QObject::tr("串联")?QObject::tr("并联"):QObject::tr("串联"));

}

QString cls6500MultiMeterMode::getTest1Level()
{
    doubleType dt;
    if(test1LevelType =="V")
    {
        test1LevelV = checkLevel(test1LevelV,test1Freq,test1LevelType);
        dt.setData(test1LevelV);
        return dt.formateToString()+"V";
    }
    else
    {
        test1LevelA = checkLevel(test1LevelA,test1Freq,test1LevelType);
        dt.setData(test1LevelA);
        return dt.formateToString()+"A";
    }
}

void cls6500MultiMeterMode::setTest1Level()
{

    dlgLevelInputBox *dlg = new dlgLevelInputBox;
    // dlg->setAVisible(false);
    dlg->setWindowTitle(QObject::tr("设置电平"));

    doubleType dt;

    if(test1LevelType=="V")
        dt.setData(test1LevelV,"");
    else
        dt.setData(test1LevelA,"");

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
            test1LevelV = dt.Data();

        }
        else
        {
            test1LevelA = dt.Data();
        }

        test1LevelType = unit;
    }
}

QString cls6500MultiMeterMode::getTest1Freq()
{
    doubleType dt;
    test1Freq = checkFreq(test1Freq);
    dt.setData(test1Freq);
    return dt.formateToString()+"Hz";
}

void cls6500MultiMeterMode::setTest1Freq()
{
    NumberInput * dlg = new NumberInput();
    dlg->setWindowTitle(QObject::tr("输入频率"));
    dlg->setValueAndSuffix(this->test1Freq,"");

    if(dlg->exec()== QDialog::Accepted)
    {
        this->test1Freq = dlg->getNumber();
    }
}

double cls6500MultiMeterMode::checkLevel(double value, double freq,QString type)
{
    if(type =="V")
    {
        value = qMax(0.01,value);
        if(freq<=5.0E7)
            value = qMin(value, 1.0);
        else if(freq <= 120.0E6)
            value = qMin(value,0.50);
        else
            value = qMin(value,1.0);
    }
    else
    {
        value = qMax(200.0E-6, value);
        if(freq<=5.0E7)
            value = qMin(value, 20.0E-3);
        else if(freq <=120.0E6)
            value = qMin(value,0.01);
        else
            value = qMin(value, 20.0E-3);
    }

    return value;
}


double cls6500MultiMeterMode::checkFreq(double value)
{
    value = qMin(value, getMaxFreq());
    value = qMax(value, 20.0);
    return value;
}

double cls6500MultiMeterMode::getMaxFreq()
{
    QString value = clsRS::getInst().instrumentModel;
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


QStringList cls6500MultiMeterMode::convertGpib()
{
    QStringList gpibCmd;
    QString meter = ":METER:";
    gpibCmd.append(meter+"FUNC:1 Z;2 ANGLE");

    if(this->range ==QObject::tr("自动"))
        gpibCmd.append(meter+"RANGE AUTO");//档位
    else
        gpibCmd.append(meter+"RANGE "+this->range);


    if(this->speed == QObject::tr("最快"))                        //速度
        gpibCmd.append(meter+"SPEED "+"MAX" );
    else if(speed == QObject::tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST" );
    else if(speed == QObject::tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED" );
    else if(speed == QObject::tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW" );
    else
        gpibCmd.append(meter+"SPEED "+"MAX" );


    return gpibCmd;

}
