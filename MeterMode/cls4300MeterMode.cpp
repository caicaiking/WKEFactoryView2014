#include "cls4300MeterMode.h"
#include "clsRuningSettings.h"
#include <QDebug>
#include "frmSetLimit.h"
#include "clsMeterLimit.h"
#include "UserfulFunctions.h"
#include "dlgLevelInputBox.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "dlg4300FuctionMajor.h"
#include "dlg4300FunctionMinor.h"
cls4300MeterMode::cls4300MeterMode(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    //链接上下限按钮的信号。
    connect(this->btnTest1Limit1,SIGNAL(Clicked()),this,SLOT(setTest1Limit1()));
    connect(this->btnTest2Limit1,SIGNAL(Clicked()),this,SLOT(setTest2Limit1()));
    connect(this->btnTest1Limit2,SIGNAL(Clicked()),this,SLOT(setTest1Limit2()));
    connect(this->btnTest2Limit2,SIGNAL(Clicked()),this,SLOT(setTest2Limit2()));


    this->items<<"Z"<<"A"<<"L"<<"Q";
    itemUnits<<tr("无")<<tr("无")<<tr("无")<<tr("无");
    clsMeterLimit m1,m2,m3,m4;
    meterLimit<<m1<<m2<<m3<<m4;
    frequency<<1000<<10000;
    level<<0.5<<1;
    equcct<<tr("串联")<<tr("并联");
    range =tr("自动");
    speed =tr("最快");
    biasSource =tr("内置");
    biasStatus = tr("关");
    isTest2On=false;

    updateButtons();

}

void cls4300MeterMode::updateButtons()
{
    groupBox->setChecked(isTest2On);
    btnTest1Item1->setText(items.at(0));
    if(btnTest1Item1->text().toUpper()==tr("Rdc").toUpper())
    {
        grpTest1Item2->setEnabled(false);
        grpTest1Item2->setVisible(false);
    }
    else
    {
        grpTest1Item2->setEnabled(true);
        grpTest1Item2->setVisible(true);
    }


    btnTest1Item2->setText(items.at(1));
    btnTest2Item1->setText(items.at(2));
    if(btnTest2Item1->text().toUpper()==tr("Rdc").toUpper())
    {
        grpTest2Item2->setEnabled(false);
        grpTest2Item2->setVisible(false);
    }
    else
    {
        grpTest2Item2->setEnabled(true && groupBox->isChecked());
        grpTest2Item2->setVisible(true);
    }

    btnTest2Item2->setText(items.at(3));

    clsMeterLimit tmp;
    tmp = meterLimit.at(0);
    btnTest1Limit1->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest1Item1->text())));

    tmp = meterLimit.at(1);
    btnTest1Limit2->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest1Item2->text())));

    tmp = meterLimit.at(2);
    btnTest2Limit1->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest2Item1->text())));

    tmp = meterLimit.at(3);
    btnTest2Limit2->setString(tmp.showLimits(UserfulFunctions::getSuffix( btnTest2Item2->text())));

    doubleType dt;
    dt.setData(frequency.at(0),"");
    btnTest1Frequency->setText(dt.formateToString()+"Hz");

    dt.setData(frequency.at(1),"");
    btnTest2Frequency->setText(dt.formateToString()+"Hz");

    dt.setData(level.at(0),"");
    btnTest1Level->setText(dt.formateToString()+"V");
    dt.setData(level.at(1),"");
    btnTest2Level->setText(dt.formateToString()+"V");

    btnTest1Range->setText(this->range);
    btnTest1Speed->setText(this->speed);

    btnTest1Equcct->setText(equcct.at(0));
    btnTest2Equcct->setText(equcct.at(1));

    cmbTest1Item1Unit->setCurrentText(itemUnits.at(0));
    cmbTest1Item2Unit->setCurrentText(itemUnits.at(1));
    cmbTest2Item1Unit->setCurrentText(itemUnits.at(2));
    cmbTest2Item2Unit->setCurrentText(itemUnits.at(3));

    btnBiasSource->setText(biasSource);
    btnBiasStatus->setText(biasStatus);

}

FunctionType cls4300MeterMode::getType()
{
    return MeterFunction;
}

void cls4300MeterMode::setCondition(const QString &strConditon)
{
    readTestCondition(strConditon);
}

QString cls4300MeterMode::getTestItem()
{

}

QString cls4300MeterMode::trig()
{
    QStringList test1Command = convertTest1Gpib();
    QStringList test2Command = convertTest2Gpib();
    if(isTest2On)
    {
        QString tmpString =":MEAS:NUMber-OF-TESTS 2";
        if(clsRS::getInst().gpibCommands.NumberOfTest!=tmpString)
        {
            clsRS::getInst().sendCommand(tmpString,false);
            clsRS::getInst().gpibCommands.NumberOfTest =tmpString;
        }

        //更新Test1Gpib指令
        if(clsRS::getInst().gpibCommands.gpibTest1!=test1Command)
        {

            if(clsRS::getInst().gpibCommands.gpibTest1.length()!= test1Command.length())
            {
                clsRS::getInst().sendCommand(":MEAS:TEST 1",false);
                foreach (QString item, test1Command) {
                    clsRS::getInst().sendCommand(item,false);
                }
            }
            else
            {
                clsRS::getInst().sendCommand(":MEAS:TEST 1",false);
                for(int i=0; i< test1Command.length();i++)
                {
                    if(clsRS::getInst().gpibCommands.gpibTest1.at(i)!=test1Command.at(i))
                        clsRS::getInst().sendCommand(test1Command.at(i));
                }
            }
            clsRS::getInst().gpibCommands.gpibTest1 = test1Command;
        }

        //更新Test2Gpib指令
        if(clsRS::getInst().gpibCommands.gpibTest2!=test2Command)
        {
            if(clsRS::getInst().gpibCommands.gpibTest2.length()!= test2Command.length())
            {
                clsRS::getInst().sendCommand(":MEAS:TEST 2",false);
                foreach (QString item, test2Command) {
                    clsRS::getInst().sendCommand(item,false);
                }
            }
            else
            {
                clsRS::getInst().sendCommand(":MEAS:TEST 2",false);
                for(int i=0; i< test2Command.length();i++)
                {
                    if(clsRS::getInst().gpibCommands.gpibTest2.at(i)!=test2Command.at(i))
                        clsRS::getInst().sendCommand(test2Command.at(i));
                }
            }

            clsRS::getInst().gpibCommands.gpibTest2=test2Command;
        }
    }
    else
    {
        QString tmpString =":MEAS:NUMber-OF-TESTS 1";
        if(clsRS::getInst().gpibCommands.NumberOfTest!=tmpString)
        {
            clsRS::getInst().sendCommand(tmpString,false);
            clsRS::getInst().gpibCommands.NumberOfTest =tmpString;
        }

        //更新Test2Gpib指令
        if(clsRS::getInst().gpibCommands.gpibTest1!=test1Command)
        {
            if(clsRS::getInst().gpibCommands.gpibTest1.length()!= test1Command.length())
            {
                foreach (QString item, test1Command) {
                    clsRS::getInst().sendCommand(item,false);
                }
            }
            else
            {
                for(int i=0; i< test1Command.length();i++)
                {
                    if(clsRS::getInst().gpibCommands.gpibTest1.at(i)!=test1Command.at(i))
                        clsRS::getInst().sendCommand(test1Command.at(i));
                }
            }

            clsRS::getInst().gpibCommands.gpibTest1 = test1Command;
        }
    }

    //如果是打开了Bias
    if(this->biasStatus==tr("开"))
        clsRS::getInst().sendCommand(":MEAS:BIAS ON",false);

    QString strRes= clsRS::getInst().sendCommand(":MEAS:TRIG",true);

    //为了开关不跳火
    if(this->biasStatus==tr("开"))
        clsRS::getInst().sendCommand(":MEAS:BIAS OFF",false);

    QVariantMap Test= convertToJson();
    Test.insert("Result", strRes);
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(Test);
    if (!jsonDocument.isNull())
        return jsonDocument.toJson(/*QJsonDocument::Compact*/);
    else
        return "";
}

QStringList cls4300MeterMode::convertTest1Gpib()
{
    QStringList gpibCmd;
    QString meter=":MEAS:";

    QString item11=( items.at(0)==QString("θ")?"A":items.at(0));
    gpibCmd.append(meter+"FUNC1 "+ item11.toUpper() );  //item1 GPIB

    QString item22=(items.at(1)==QString("θ")?"A":items.at(1));
    gpibCmd.append(meter+"FUNC2 "+item22.toUpper() );  //Item2 GPIB

    if(equcct.at(0)==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER" );
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR" );

    if(range ==tr("自动"))                        //档位
        gpibCmd.append(meter+"RANGE "+"AUTO" );
    else
        gpibCmd.append(meter+"RANGE "+range );

    if(speed ==tr("最快"))                        //速度
        gpibCmd.append(meter+"SPEED "+"MAX" );
    else if(speed ==tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST" );
    else if(speed ==tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED" );
    else if(speed ==tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW" );
    else
        gpibCmd.append(meter+"SPEED "+"MAX" );

    doubleType dt;
    dt.setData(frequency.at(0),"");

    gpibCmd.append(meter+"FREQ "+dt.formateWithUnit("",10) );   //频率

    gpibCmd.append(meter+"LEV "+QString::number(level.at(0)) ); //电平


    if(biasSource==tr("内置"))
        gpibCmd.append(meter+"BIAS "+"VINT" );

    else
        gpibCmd.append(meter+"BIAS "+"VEXT" );



    return gpibCmd;

}

QStringList cls4300MeterMode::convertTest2Gpib()
{
    QStringList gpibCmd;
    QString meter=":MEAS:";

    QString item11=( items.at(2)==QString("θ")?"A":items.at(2));
    gpibCmd.append(meter+"FUNC1 "+ item11 );  //item1 GPIB

    QString item22=(items.at(3)==QString("θ")?"A":items.at(3));
    gpibCmd.append(meter+"FUNC2 "+item22 );  //Item2 GPIB

    if(equcct.at(1)==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER" );
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR" );



    doubleType dt;
    dt.setData(frequency.at(1),"");

    gpibCmd.append(meter+"FREQ "+dt.formateWithUnit("",10) );   //频率
    gpibCmd.append(meter+"LEV "+QString::number(level.at(1)) ); //电平
    return gpibCmd;
}

QVariantMap cls4300MeterMode::convertToJson()
{
    QVariantMap test1;
    clsMeterLimit tmp;
    /*1*/  test1.insert("Item1",items.at(0));
    /*2*/  test1.insert("Unit1",itemUnits.at(0));

    tmp =  meterLimit.at(0);
    /*3*/  test1.insert("Limit1",tmp.toString());

    if(items.at(1).toUpper()==tr("OFF") || items.at(0).toUpper() == tr("RDC"))
        /*4*/     test1.insert("Item2","");
    else
        /*4*/     test1.insert("Item2",items.at(1));

    /*5*/ test1.insert("Unit2", itemUnits.at(1));
    tmp = meterLimit.at(1);
    /*6*/ test1.insert("Limit2",tmp.toString());

    QVariantMap test2;


    /*7*/  test2.insert("Item1",items.at(2));
    /*8*/  test2.insert("Unit1",itemUnits.at(2));

    tmp =  meterLimit.at(2);
    /*9*/  test2.insert("Limit1",tmp.toString());

    if(items.at(3).toUpper()==tr("OFF") || items.at(2).toUpper() == tr("RDC"))
        /*10*/     test2.insert("Item2","");
    else
        /*10*/     test2.insert("Item2",items.at(3));

    /*11*/ test2.insert("Unit2", itemUnits.at(3));
    tmp = meterLimit.at(3);
    /*12*/ test2.insert("Limit2",tmp.toString());
    ///*13*/ test2.insert("IsOnTest2",this->isTest2On);

    QVariantMap Test;

    Test.insert("test1",test1);
    Test.insert("test2",test2);
    Test.insert("IsOnTest2",this->isTest2On);
    return Test;

}

void cls4300MeterMode::start()
{

}

QString cls4300MeterMode::getTestCondition()
{
    return saveTestConditons();
}

QString cls4300MeterMode::getBrief()
{

}

bool cls4300MeterMode::getPassFail()
{

}

void cls4300MeterMode::turnOffBias()
{

    clsRS::getInst().sendCommand(":MEAS:BIAS OFF",false);

}

void cls4300MeterMode::setTest1Limit1()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(0));
    dlg->setItem(btnTest1Item1->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(0,dlg->getMeterLimit());
        this->btnTest1Limit1->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest1Item1->text())));
    }
}

void cls4300MeterMode::setTest2Limit1()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(2));
    dlg->setItem(btnTest2Item1->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(2,dlg->getMeterLimit());
        this->btnTest2Limit1->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest2Item1->text())));
    }
}

void cls4300MeterMode::setTest1Limit2()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(1));
    dlg->setItem(btnTest1Item2->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(1,dlg->getMeterLimit());
        this->btnTest1Limit2->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest1Item2->text())));
    }
}

void cls4300MeterMode::setTest2Limit2()
{
    frmSetLimit * dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定上限和下限"));

    dlg->setLimits(meterLimit.at(3));
    dlg->setItem(btnTest2Item2->text());
    if(dlg->exec()==QDialog::Accepted)
    {
        //获取设定的上下限。
        meterLimit.replace(3,dlg->getMeterLimit());
        this->btnTest2Limit2->setText(dlg->getMeterLimit().showLimits(UserfulFunctions::getSuffix(this->btnTest2Item2->text())));
    }
}

double cls4300MeterMode::getMaxFrequency1(QString value)
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

void cls4300MeterMode::on_btnTest1Frequency_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入频率"));

    if(dlg->exec())
    {
        double value = dlg->getNumber();
        double maxFreuqncy = getMaxFrequency1(clsRS::getInst().instrumentModel);
        value =(value>maxFreuqncy? maxFreuqncy:value);
        value = (value<20? 20:value);
        frequency.replace(0,value);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Frequency_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入频率"));

    if(dlg->exec())
    {
        double value = dlg->getNumber();
        double maxFreuqncy = getMaxFrequency1(clsRS::getInst().instrumentModel);
        value =(value>maxFreuqncy? maxFreuqncy:value);
        value = (value<20? 20:value);
        frequency.replace(1,value);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Level_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox(this);
    dlg->setAVisible(false);
    dlg->setWindowTitle("输入电平");

    if(dlg->exec())
    {
        double value=dlg->getValue();
        QString suffix=dlg->getSuffix();
        doubleType dt;
        dt.setData(value,suffix);

        double number = dt.Data();
        number=(number>2?2:number);
        number=(number<0.01?0.01:number);
        level.replace(0,number);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Level_clicked()
{
    dlgLevelInputBox *dlg = new dlgLevelInputBox(this);
    dlg->setAVisible(false);
    dlg->setWindowTitle("输入电平");

    if(dlg->exec())
    {
        double value=dlg->getValue();
        QString suffix=dlg->getSuffix();
        doubleType dt;
        dt.setData(value,suffix);

        double number = dt.Data();
        number=(number>2?2:number);
        number=(number<0.01?0.01:number);
        level.replace(1,number);
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Range_clicked()
{
    wk6500Range *dlg = new wk6500Range(this);
    dlg->setWindowTitle(tr("设置测试速度"));
    if(dlg->exec())
    {
        range = dlg->getRange();
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Speed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置测试速度"));

    if(dlg->exec())
    {
        this->speed= dlg->getSpeed();
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Equcct_clicked()
{
    if(equcct.at(0)==tr("串联"))
    {
        equcct.replace(0,tr("并联"));
    }
    else
    {
        equcct.replace(0,tr("串联"));
    }
    updateButtons();
}

void cls4300MeterMode::on_btnTest2Equcct_clicked()
{
    if(equcct.at(1)==tr("串联"))
    {
        equcct.replace(1,tr("并联"));
    }
    else
    {
        equcct.replace(1,tr("串联"));
    }
    updateButtons();
}

void cls4300MeterMode::on_btnTest1Item1_clicked()
{
    dlg4300FuctionMajor *dlg = new dlg4300FuctionMajor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(0, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest1Item2_clicked()
{
    dlg4300FunctionMinor *dlg = new dlg4300FunctionMinor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(1, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Item1_clicked()
{
    dlg4300FuctionMajor *dlg = new dlg4300FuctionMajor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(2, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnTest2Item2_clicked()
{
    dlg4300FunctionMinor *dlg = new dlg4300FunctionMinor(this);
    dlg->setWindowTitle(tr("测试项目"));

    if(dlg->exec())
    {
        items.replace(3, dlg->getItem());
        updateButtons();
    }
}

void cls4300MeterMode::on_btnBiasSource_clicked()
{
    if(biasSource==tr("内置"))
    {
        biasSource=tr("外置");
    }
    else
    {
        biasSource=tr("内置");
    }
    updateButtons();
}

void cls4300MeterMode::on_btnBiasStatus_clicked()
{
    if(biasStatus==tr("开"))
    {
        biasStatus=tr("关");
    }
    else
    {
        biasStatus=tr("开");
    }
    updateButtons();
}

QString cls4300MeterMode::saveTestConditons()
{
    QVariantMap tmpMap;

    tmpMap.insert("Instument","WK4300");

    tmpMap.insert("Frequency",UserfulFunctions::converToQString(this->frequency));
    tmpMap.insert("Item1",UserfulFunctions::converToQString(this->items));
    tmpMap.insert("ItemUnits",UserfulFunctions::converToQString(this->itemUnits));
    tmpMap.insert("Level",UserfulFunctions::converToQString(this->level));
    tmpMap.insert("Equcct",UserfulFunctions::converToQString(this->equcct));

    QStringList tmpList;
    for(int i=0;i<this->meterLimit.length();i++)
    {
        tmpList.append(meterLimit[i].toString());
    }
    tmpMap.insert("MeterLimit",UserfulFunctions::converToQString(tmpList,'|'));

    tmpMap.insert("Range",this->range);
    tmpMap.insert("Speed",this->speed);
    tmpMap.insert("BiasSource",this->biasSource);
    tmpMap.insert("BiasStatus",this->biasStatus);
    tmpMap.insert("IsTest2On",this->isTest2On);


    QJsonDocument jsDocument = QJsonDocument::fromVariant(tmpMap);
    if(!jsDocument.isNull())
    {
        return jsDocument.toJson();
    }
    return "";
}

void cls4300MeterMode::readTestCondition(QString strCondtion)
{
    QJsonParseError error;
    QJsonDocument jsDocument=QJsonDocument::fromJson(strCondtion.toUtf8(),&error);
    if(error.error==QJsonParseError::NoError)
    {
        if(jsDocument.isObject())
        {
            QVariantMap result = jsDocument.toVariant().toMap();

            QString instrument = result["Instument"].toString();
            if(instrument!="WK4300")
                return;

            range = result["Range"].toString();
            speed = result["Speed"].toString();
            biasSource = result["BiasSource"].toString();
            biasStatus = result["BiasStatus"].toString();
            isTest2On =result["IsTest2On"].toBool();

            QString tmp ;
            tmp = result["Frequency"].toString();
            this->frequency = UserfulFunctions::converToDoubleList(tmp);

            tmp = result["Item1"].toString();
            this->items = tmp.split(",");

            tmp = result["ItemUnits"].toString();
            this->itemUnits = tmp.split(",");

            tmp = result["Level"].toString();
            this->level = UserfulFunctions::converToDoubleList(tmp);

            tmp = result["Equcct"].toString();
            this->equcct = tmp.split(",");

            tmp = result["MeterLimit"].toString();
            QStringList tmpList = tmp.split('|');

            if(tmpList.length()== meterLimit.length() )
            {
                for(int i=0; i<tmpList.length();i++)
                {
                    meterLimit[i].setString(tmpList.at(i));
                }
            }

        }
    }



    updateButtons();
}

void cls4300MeterMode::on_cmbTest1Item1Unit_currentIndexChanged(const QString &arg1)
{
    itemUnits.replace(0,arg1);
    updateButtons();
}

void cls4300MeterMode::on_cmbTest1Item2Unit_currentIndexChanged(const QString &arg1)
{
    itemUnits.replace(1,arg1);
    updateButtons();
}

void cls4300MeterMode::on_cmbTest2Item1Unit_currentIndexChanged(const QString &arg1)
{
    itemUnits.replace(2,arg1);
    updateButtons();
}

void cls4300MeterMode::on_cmbTest2Item2Unit_currentIndexChanged(const QString &arg1)
{
    itemUnits.replace(3,arg1);
    updateButtons();
}

void cls4300MeterMode::on_groupBox_clicked()
{
    isTest2On = groupBox->isChecked();
    updateButtons();
}

void cls4300MeterMode::on_btnOk_clicked()
{
    this->close();
}
