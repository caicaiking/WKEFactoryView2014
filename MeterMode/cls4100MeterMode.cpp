#include "cls4100MeterMode.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "dlg4300FuctionMajor.h"
#include "dlg4300FunctionMinor.h"
#include "clsMeterUnit.h"
#include "frmSetLimit.h"
#include "NumberInput.h"
#include "dlgLevelInputBox.h"
#include "clsRuningSettings.h"
#include "wk6500Range.h"
#include "dlgSpeed.h"
#include "cls4100MeterModeCalibration.h"
cls4100MeterMode::cls4100MeterMode(QWidget *parent) :
    WKEMeterMode(parent)
{
    setupUi(this);


    connect(this->btnTest1Limit1,SIGNAL(Clicked()),this,SLOT(btnTest1Limit1clicked()));
    connect(this->btnTest1Limit2,SIGNAL(Clicked()),this,SLOT(btnTest1Limit2clicked()));
    connect(this->btnTest2Limit1,SIGNAL(Clicked()),this,SLOT(btnTest2Limit1clicked()));
    connect(this->btnTest2Limit2,SIGNAL(Clicked()),this,SLOT(btnTest2Limit2clicked()));

    test1Item1 ="Z";
    test1Item2 ="A";
    test2Item1 ="Z";
    test2Item2 ="A";

    item1Suffix ="";
    item2Suffix ="";
    item3Suffix ="";
    item4Suffix ="";

    levelTest1Ac=1;
    levelTest1Rdc=1;
    levelTest2Ac=1;
    levelTest2Rdc=1;

    test1Freq=10000;
    test2Freq=100000;

    test1Equcct=tr("串联");
    test2Equcct=tr("串联");

    test1Range=tr("自动");
    test1Speed=tr("最快");

    updateButtons();
}

void cls4100MeterMode::updateButtons()
{
    btnTest1Item1->setText(test1Item1);
    btnTest2Item1->setText(test2Item1);
    doubleType dt;

    if(test1Item1.toUpper()=="RDC")
    {
        grpTest1Item2->setVisible(false);
        btnTest1Equcct->setVisible(false);
        lblTest1Freq->setVisible(false);
        btnTest1Frequency->setVisible(false);
        dt.setData(levelTest1Rdc);
        btnTest1Level->setText(dt.formateToString()+"V");
    }
    else
    {
        grpTest1Item2->setVisible(true);
        btnTest1Equcct->setVisible(true);
        lblTest1Freq->setVisible(true);
        btnTest1Frequency->setVisible(true);
        dt.setData(levelTest1Ac);
        btnTest1Level->setText(dt.formateToString()+"V");

    }

    if(test2Item1.toUpper()=="RDC")
    {
        grpTest2Item2->setVisible(false);
        lblTest2Equcct->setVisible(false);
        btnTest2Equcct->setVisible(false);
        lblTest2Freq->setVisible(false);
        btnTest2Frequency->setVisible(false);
        dt.setData(levelTest2Rdc);
        btnTest2Level->setText(dt.formateToString()+"V");
    }
    else
    {
        grpTest2Item2->setVisible(true);
        btnTest2Equcct->setVisible(true);
        lblTest2Equcct->setVisible(true);
        lblTest2Freq->setVisible(true);
        btnTest2Frequency->setVisible(true);
        dt.setData(levelTest2Ac);
        btnTest2Level->setText(dt.formateToString()+"V");
    }

    if(test1Item2.toUpper()=="OFF")
    {
        btnTest1Limit2->setEnabled(false);
        btnTest1Item2Unit->setEnabled(false);
    }
    else
    {
        btnTest1Limit2->setEnabled(true);
        btnTest1Item2Unit->setEnabled(true);
    }


    if(test2Item2.toUpper()=="OFF")
    {
        btnTest2Limit2->setEnabled(false);
        btnTest2Item2Unit->setEnabled(false);
    }
    else
    {
        btnTest2Limit2->setEnabled(true);
        btnTest2Item2Unit->setEnabled(true);
    }

    btnTest2Item2->setText(test2Item2);
    btnTest1Item2->setText(test1Item2);
    dt.setData(test1Freq);
    btnTest1Frequency->setText(dt.formateToString()+"Hz");
    dt.setData(test2Freq);
    btnTest2Frequency->setText(dt.formateToString()+"Hz");

    btnTest1Equcct->setText(test1Equcct);
    btnTest2Equcct->setText(test2Equcct);
    btnTest1Speed->setText(test1Speed);
    btnTest1Range->setText(test1Range);

    btnTest1Limit1->setText(mlItem1.showLimits(UserfulFunctions::getSuffix( test1Item1)));
    btnTest1Limit2->setText(mlItem2.showLimits(UserfulFunctions::getSuffix( test1Item2)));
    btnTest2Limit1->setText(mlItem3.showLimits(UserfulFunctions::getSuffix( test2Item1)));
    btnTest2Limit2->setText(mlItem4.showLimits(UserfulFunctions::getSuffix( test2Item2)));

    btnTest1Item1Unit->setText(item1Suffix+UserfulFunctions::getSuffix(test1Item1));
    btnTest1Item2Unit->setText(item2Suffix+UserfulFunctions::getSuffix(test1Item2));
    btnTest2Item1Unit->setText(item3Suffix+UserfulFunctions::getSuffix(test2Item1));
    btnTest2Item2Unit->setText(item4Suffix+UserfulFunctions::getSuffix(test2Item2));
}

bool cls4100MeterMode::detectDut()
{


    if(clsRS::getInst().gpibCommands.NumberOfTest==":MEAS:NUMber-OF-TESTS 2")
    {
        clsRS::getInst().sendCommand(":MEAS:NUMber-OF-TESTS 1",false);
    }

    isStop= true;
    bool isEmpty=false;

    QString item;


    int index = clsRS::getInst().sendCommand(":MEAS:FUNC1?",true).toInt();

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
    case 6:
        item="Q";
        break;
    case 7:
        item ="D";
        break;
    case 8:
        item="R";
        break;
    case 9:
        item ="G";
        break;
    case 10:
        item ="A";
        break;
    case 11:
        item ="RDC";
        break;
    default:
        break;
    }

    if(item!="C")
        clsRS::getInst().sendCommand(QString(":MEAS:FUNC1 C"),false);


    while(isStop)
    {
        QString retValue = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

        double value = retValue.split(",").at(0).toDouble();

        if(qAbs(value)>5E-12)
        {
            if(isEmpty)
            {
                clsRS::getInst().sendCommand(QString(":MEAS:FUNC1 %1").arg(item),false);
                clsRS::getInst().sendCommand(clsRS::getInst().gpibCommands.NumberOfTest,false);
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
emit detectInProgress("");
    return false;

}

void cls4100MeterMode::stopDetect()
{
    isStop = false;

    qApp->processEvents();
}

void cls4100MeterMode::setCondition(QString value)
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
                this->test1Item1 = result["test1Item1"].toString();
                this->test1Item2 = result["test1Item2"].toString();
                this->test2Item1 = result["test2Item1"].toString();
                this->test2Item2 = result["test2Item2"].toString();
                this->test1Freq = result["test1Freq"].toDouble();
                this->test2Freq = result["test2Freq"].toDouble();
                this->item1Suffix = result["item1Suffix"].toString();
                this->item2Suffix = result["item2Suffix"].toString();
                this->item3Suffix = result["item3Suffix"].toString();
                this->item4Suffix = result["item4Suffix"].toString();
                this->mlItem1.setString(result["mlItem1"].toString());
                this->mlItem2.setString(result["mlItem2"].toString());
                this->mlItem3.setString(result["mlItem3"].toString());
                this->mlItem4.setString(result["mlItem4"].toString());
                this->levelTest1Ac=result["levelTest1Ac"].toDouble();
                this->levelTest1Rdc = result["levelTest1Rdc"].toDouble();
                this->levelTest2Ac=result["levelTest2Ac"].toDouble();
                this->levelTest2Rdc = result["levelTest2Rdc"].toDouble();
                this->test1Equcct =result["test1Equcct"].toString();
                this->test2Equcct =result["test2Equcct"].toString();
                this->test1Range = result["test1Range"].toString();
                this->test1Speed = result["test1Speed"].toString();
                this->grpTest2->setChecked(result["isTest2On"].toBool());
                this->txtDescription->setText(result["description"].toString());
                updateButtons();
            }
        }
    }
}

QString cls4100MeterMode::getConditon()
{
    QVariantMap step;

    step.insert("test1Item1",this->test1Item1);
    step.insert("test1Item2",this->test1Item2);
    step.insert("test2Item1",this->test2Item1);
    step.insert("test2Item2",this->test2Item2);
    step.insert("test1Freq",this->test1Freq);
    step.insert("test2Freq",this->test2Freq);
    step.insert("item1Suffix",this->item1Suffix);
    step.insert("item2Suffix",this->item2Suffix);
    step.insert("item3Suffix",this->item3Suffix);
    step.insert("item4Suffix",this->item4Suffix);
    step.insert("mlItem1",this->mlItem1.toString());
    step.insert("mlItem2",this->mlItem2.toString());
    step.insert("mlItem3",this->mlItem3.toString());
    step.insert("mlItem4",this->mlItem4.toString());
    step.insert("levelTest1Ac",this->levelTest1Ac);
    step.insert("levelTest1Rdc",this->levelTest1Rdc);
    step.insert("levelTest2Ac",this->levelTest2Ac);
    step.insert("levelTest2Rdc",this->levelTest2Rdc);
    step.insert("test1Equcct",this->test1Equcct);
    step.insert("test2Equcct",this->test2Equcct);
    step.insert("test1Range",this->test1Range);
    step.insert("test1Speed",this->test1Speed);
    step.insert("isTest2On",this->grpTest2->isChecked());
    step.insert("description",this->txtDescription->text());

    QJsonDocument jsonDocument  = QJsonDocument::fromVariant(step);

    if(!jsonDocument.isNull())
        return jsonDocument.toJson();
    else
        return "";

}



QStringList cls4100MeterMode::convertTest1Gpib()
{
    QStringList gpibCmd;
    QString meter=":MEAS:";

    gpibCmd.append(meter+"FUNC1 "+ test1Item1);  //item1 GPIB


    gpibCmd.append(meter+"FUNC2 "+test1Item2);  //Item2 GPIB

    if(test1Equcct==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER" );
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR" );

    if(test1Range ==tr("自动"))                        //档位
        gpibCmd.append(meter+"RANGE "+"AUTO" );
    else
        gpibCmd.append(meter+"RANGE "+test1Range );

    if(test1Speed ==tr("最快"))                        //速度
        gpibCmd.append(meter+"SPEED "+"MAX" );
    else if(test1Speed ==tr("快速"))
        gpibCmd.append(meter+"SPEED "+"FAST" );
    else if(test1Speed ==tr("中速"))
        gpibCmd.append(meter+"SPEED "+"MED" );
    else if(test1Speed ==tr("慢速"))
        gpibCmd.append(meter+"SPEED "+"SLOW" );
    else
        gpibCmd.append(meter+"SPEED "+"MAX" );

    doubleType dt;
    dt.setData(test1Freq,"");

    gpibCmd.append(meter+"FREQ "+dt.formateWithUnit("",10) );   //频率
    if(test1Item1.toUpper()!="RDC")
        gpibCmd.append(meter+"LEV "+QString::number(levelTest1Ac) ); //电平
    else
        gpibCmd.append(meter+"LEV "+QString::number(levelTest1Rdc) ); //电平

    return gpibCmd;

}

QStringList cls4100MeterMode::convertTest2Gpib()
{
    QStringList gpibCmd;
    QString meter=":MEAS:";

    gpibCmd.append(meter+"FUNC1 "+ test2Item1 );  //item1 GPIB
    gpibCmd.append(meter+"FUNC2 "+test2Item2 );  //Item2 GPIB

    if(test2Equcct==tr("串联"))                       //等效电路
        gpibCmd.append(meter+"EQU-CCT "+"SER" );
    else
        gpibCmd.append(meter+"EQU-CCT "+"PAR" );



    doubleType dt;
    dt.setData(test2Freq,"");

    gpibCmd.append(meter+"FREQ "+dt.formateWithUnit("",10) );   //频率
    if(test2Item1.toUpper()!="RDC")
        gpibCmd.append(meter+"LEV "+QString::number(levelTest2Ac) ); //电平
    else
        gpibCmd.append(meter+"LEV "+QString::number(levelTest2Rdc) ); //电平
    return gpibCmd;
}


void cls4100MeterMode::updateGPIB()
{
    QStringList test1Command = convertTest1Gpib();
    QStringList test2Command = convertTest2Gpib();
    if(grpTest2->isChecked())
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

}

bool cls4100MeterMode::getTotalStatus()
{
    bool status = true;

    for(int i=0; i< this->getCountTestItems();i++)
    {
        double data = getResult(i);
        clsMeterLimit lim = getLimit(i);

        status = status && lim.comparaValue(data);
    }

    return status;
}

int cls4100MeterMode::getCountTestItems()
{
    int test1Count=0;
    int test2Count=0;

    if(test1Item1.toUpper()=="RDC")
        test1Count=1;
    else
    {
        if(test1Item2.toUpper()=="OFF")
        {
            test1Count=1;
        }
        else
        {
            test1Count=2;
        }
    }

    if(grpTest2->isChecked())
    {
        if(test2Item1.toUpper()=="RDC")
        {
            test2Count=1;
        }
        else
        {
            if(test2Item2.toUpper()=="OFF")
            {
                test2Count=1;
            }
            else
            {
                test2Count=2;
            }
        }
    }
    else
    {
        test2Count=0;
    }

    return test1Count+test2Count;

}

QString cls4100MeterMode::getItem(int i)
{

    QStringList test1;
    QStringList test2;

    if(test1Item1.toUpper()=="RDC")
        test1.append(test1Item1);
    else
    {
        if(test1Item2.toUpper()=="OFF")
        {
            test1.append(test1Item1);
        }
        else
        {
            test1.append(test1Item1);
            test1.append(test1Item2);
        }
    }

    if(grpTest2->isChecked())
    {
        if(test2Item1.toUpper()=="RDC")
        {
            test2.append(test2Item1);
        }
        else
        {
            if(test2Item2.toUpper()=="OFF")
            {
                test2.append(test2Item1);
            }
            else
            {
                test2.append(test2Item1);
                test2.append(test2Item2);
            }
        }
    }

    QStringList totle;
    totle.append(test1);
    totle.append(test2);

    return totle.at(i);
}

clsMeterLimit cls4100MeterMode::getLimit(int i)
{
    QList<clsMeterLimit> test1;
    QList<clsMeterLimit> test2;

    if(test1Item1.toUpper()=="RDC")
        test1.append(mlItem1);
    else
    {
        if(test1Item2.toUpper()=="OFF")
        {
            test1.append(mlItem1);
        }
        else
        {
            test1.append(mlItem1);
            test1.append(mlItem2);
        }
    }

    if(grpTest2->isChecked())
    {
        if(test2Item1.toUpper()=="RDC")
        {
            test2.append(mlItem3);
        }
        else
        {
            if(test2Item2.toUpper()=="OFF")
            {
                test2.append(mlItem3);
            }
            else
            {
                test2.append(mlItem3);
                test2.append(mlItem4);
            }
        }
    }

    QList<clsMeterLimit> totle;
    totle.append(test1);
    totle.append(test2);

    return totle.at(i);
}

void cls4100MeterMode::calibration()
{
    cls4100MeterModeCalibration *dlg = new cls4100MeterModeCalibration(this);
    dlg->exec();
}

QString cls4100MeterMode::getBrief()
{

    QStringList condition;
    condition.append("Test1");
    if(test1Item1.toUpper()=="RDC")
    {
        condition .append(test1Item1);
        condition.append(btnTest1Level->text());
    }
    else
    {
        if(test1Item2.toUpper()=="OFF")
        {
            condition.append(test1Item1);
        }
        else
        {
            condition.append(test1Item1);
            condition.append(test1Item2);

        }
        condition.append(btnTest1Equcct->text());
        condition.append(btnTest1Frequency->text());
    }

    if(grpTest2->isChecked())
    {
        condition.append("Test2");
        if(test2Item1.toUpper()=="RDC")
        {
            condition.append(test2Item1);
            condition.append(btnTest2Level->text());
        }
        else
        {
            if(test2Item2.toUpper()=="OFF")
            {
                condition.append(test2Item1);
            }
            else
            {
                condition.append(test2Item1);
                condition.append(test2Item2);
            }
            condition.append(btnTest2Equcct->text());
            condition.append(btnTest2Frequency->text());
        }
    }


    return condition.join(",");
}

QString cls4100MeterMode::getMeterSeries()
{
    return clsRS::getInst().meterSeries;
}

QString cls4100MeterMode::getDescription()
{
    return txtDescription->text();
}

void cls4100MeterMode::turnOffBias()
{
    //这个没有用给4100，4300.
}

void cls4100MeterMode::singleTrig()
{
    QString trigCmd =":MEAS:TRIG";

    QString strRes = clsRS::getInst().sendCommand(trigCmd,true);

    qDebug()<<strRes;

    strRes+=",,,,";

    QStringList lsRes = strRes.split(",");

    item1Res = lsRes.at(0).toDouble();
    item2Res = lsRes.at(1).toDouble();
    item3Res = lsRes.at(2).toDouble();
    item4Res = lsRes.at(3).toDouble();



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
    emit signalTestResult(showTest.join(","));

}

QString cls4100MeterMode::getItemShow(const QString &item, const double &value , clsMeterLimit &limit, const QString &/*suffix*/)
{
    QStringList tmp;
    tmp<< item;

    doubleType dt;
    dt.setData(value,"");


    bool blValue = limit.comparaValue(value);
    QString strPassFail = blValue?tr("PASS"):tr("FAIL");

    if((item=="Q") || (item=="D"))
    {
        tmp.append(dt.formateWithUnit(",",7)+UserfulFunctions::getSuffix(item));
    }
    else
    {
        tmp.append(dt.formateToString(7)+UserfulFunctions::getSuffix(item));
    }
    tmp.append(strPassFail);

    return tmp.join(",");

}

void cls4100MeterMode::repetiveTrig()
{
    QString trigCmd =":MEAS:TRIG";

    QString strRes = clsRS::getInst().sendCommand(trigCmd,true);

    //qDebug()<<strRes;

    strRes+=",,,,";

    QStringList lsRes = strRes.split(",");

    item1Res = lsRes.at(0).toDouble();
    item2Res = lsRes.at(1).toDouble();
    item3Res = lsRes.at(2).toDouble();
    item4Res = lsRes.at(3).toDouble();
}

double cls4100MeterMode::getResult(int i)
{

    QList<double> test1;
    QList<double> test2;

    if(test1Item1.toUpper()=="RDC")
        test1.append(item1Res);
    else
    {
        if(test1Item2.toUpper()=="OFF")
        {
            test1.append(item1Res);
        }
        else
        {
            test1.append(item1Res);
            test1.append(item2Res);
        }
    }

    if(grpTest2->isChecked())
    {
        if(test2Item1.toUpper()=="RDC")
        {
            test2.append(item3Res);
        }
        else
        {
            if(test2Item2.toUpper()=="OFF")
            {
                test2.append(item3Res);
            }
            else
            {
                test2.append(item3Res);
                test2.append(item4Res);
            }
        }
    }

    QList<double> totle;
    totle.append(test1);
    totle.append(test2);

    return totle.at(i);

}
QString cls4100MeterMode::getSuffix(int i)
{
    QStringList test1;
    QStringList test2;

    if(test1Item1.toUpper()=="RDC")
        test1.append(item1Suffix);
    else
    {
        if(test1Item2.toUpper()=="OFF")
        {
            test1.append(item1Suffix);
        }
        else
        {
            test1.append(item1Suffix);
            test1.append(item2Suffix);
        }
    }

    if(grpTest2->isChecked())
    {
        if(test2Item1.toUpper()=="RDC")
        {
            test2.append(item3Suffix);
        }
        else
        {
            if(test2Item2.toUpper()=="OFF")
            {
                test2.append(item3Suffix);
            }
            else
            {
                test2.append(item3Suffix);
                test2.append(item4Suffix);
            }
        }
    }

    QStringList totle;
    totle.append(test1);
    totle.append(test2);

    return totle.at(i);
}

void cls4100MeterMode::on_btnTest1Frequency_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("请输入频率"));
    dlg->setValueAndSuffix(test1Freq,"");
    if(dlg->exec()==QDialog::Accepted)
    {
        test1Freq=  dlg->getNumber();
        double maxFreq =this->getMaxFrequency(clsRS::getInst().instrumentModel);
        test1Freq = test1Freq> maxFreq? maxFreq: test1Freq;
        test1Freq = test1Freq <20 ? 20: test1Freq;
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest1Level_clicked()
{
    if(test1Item1.toUpper() != "RDC")
    {
        dlgLevelInputBox * dlg = new dlgLevelInputBox(this);
        dlg->setWindowTitle(tr("设置电平"));
        dlg->setAVisible(false);
        dlg->setValueAndSuffix(this->levelTest1Ac,"");

        if(dlg->exec()==QDialog::Accepted)
        {
            double value = dlg->getValue();
            QString suffix = dlg->getSuffix();
            doubleType dt;
            dt.setData(value,suffix);

            this->levelTest1Ac = dt.Data();

            levelTest1Ac = levelTest1Ac>2 ? 2: levelTest1Ac;
            levelTest1Ac = levelTest1Ac<0.01 ?0.01 : levelTest1Ac;

            updateButtons();

        }
    }
    else
    {
        levelTest1Rdc = levelTest1Rdc != 2.0? 2.0 :1.0;
        updateButtons();
    }

}

void cls4100MeterMode::on_btnTest1Range_clicked()
{
    wk6500Range *dlg = new wk6500Range(this);
    dlg->setWindowTitle(tr("设置测试档位"));
    if(dlg->exec()==QDialog::Accepted)
    {
        this->test1Range = dlg->getRange();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest1Speed_clicked()
{
    dlgSpeed * dlg  = new dlgSpeed(this);
    if(dlg->exec()== QDialog::Accepted)
    {
        this->test1Speed = dlg->getSpeed();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest1Equcct_clicked()
{
    test1Equcct = test1Equcct ==tr("串联")? tr("并联"): tr("串联");
    updateButtons();
}

void cls4100MeterMode::on_btnTest1Item1_clicked()
{
    dlg4300FuctionMajor *dlg = new dlg4300FuctionMajor(this);
    if(dlg->exec()==QDialog::Accepted)
    {
        test1Item1 = dlg->getItem();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest1Item1Unit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(test1Item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->item1Suffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest1Item2_clicked()
{
    dlg4300FunctionMinor *dlg = new dlg4300FunctionMinor(this);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->test1Item2 = dlg->getItem();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest1Item2Unit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(test1Item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->item2Suffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest2Level_clicked()
{

    if(test2Item1.toUpper() != "RDC")
    {
        dlgLevelInputBox * dlg = new dlgLevelInputBox(this);
        dlg->setWindowTitle(tr("设置电平"));
        dlg->setAVisible(false);
        dlg->setValueAndSuffix(this->levelTest2Ac,"");

        if(dlg->exec()==QDialog::Accepted)
        {
            double value = dlg->getValue();
            QString suffix = dlg->getSuffix();
            doubleType dt;
            dt.setData(value,suffix);

            this->levelTest2Ac = dt.Data();

            levelTest2Ac = levelTest2Ac>2 ? 2: levelTest2Ac;
            levelTest2Ac = levelTest2Ac<0.01 ?0.01 : levelTest2Ac;

            updateButtons();


        }
    }
    else
    {
        levelTest2Rdc = levelTest2Rdc != 2.0? 2.0 :1.0;
        updateButtons();
    }


}

void cls4100MeterMode::on_btnTest2Frequency_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("请输入频率"));
    dlg->setValueAndSuffix(test2Freq,"");

    if(dlg->exec()==QDialog::Accepted)
    {
        test2Freq=  dlg->getNumber();
        double maxFreq =this->getMaxFrequency(clsRS::getInst().instrumentModel);
        test2Freq = test2Freq> maxFreq? maxFreq: test2Freq;
        test2Freq = test2Freq <20 ? 20: test2Freq;
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest2Equcct_clicked()
{
    test2Equcct = test2Equcct ==tr("串联")? tr("并联"):tr("串联");
    updateButtons();
}

void cls4100MeterMode::on_btnTest2Item1_clicked()
{
    dlg4300FuctionMajor *dlg = new dlg4300FuctionMajor(this);
    if(dlg->exec()==QDialog::Accepted)
    {
        test2Item1 = dlg->getItem();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest2Item1Unit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(test2Item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->item3Suffix= dlg->getSuffix();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest2Item2_clicked()
{
    dlg4300FunctionMinor *dlg = new dlg4300FunctionMinor(this);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->test2Item2 = dlg->getItem();
        updateButtons();
    }
}

void cls4100MeterMode::on_btnTest2Item2Unit_clicked()
{
    clsMeterUnit *dlg = new clsMeterUnit(this);
    dlg->setItem(test2Item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        this->item4Suffix= dlg->getSuffix();
        updateButtons();
    }

}

void cls4100MeterMode::btnTest1Limit1clicked()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlItem1);
    dlg->setItem(test1Item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        mlItem1 = dlg->getMeterLimit();
        updateButtons();
    }

}

void cls4100MeterMode::btnTest1Limit2clicked()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlItem2);
    dlg->setItem(test1Item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        mlItem2 = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls4100MeterMode::btnTest2Limit1clicked()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlItem3);
    dlg->setItem(test2Item1);
    if(dlg->exec()==QDialog::Accepted)
    {
        mlItem3 = dlg->getMeterLimit();
        updateButtons();
    }
}

void cls4100MeterMode::btnTest2Limit2clicked()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(mlItem4);
    dlg->setItem(test2Item2);
    if(dlg->exec()==QDialog::Accepted)
    {
        mlItem4 = dlg->getMeterLimit();
        updateButtons();
    }
}

double cls4100MeterMode::getMaxFrequency(QString value)
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
