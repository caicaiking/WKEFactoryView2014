#include "clsMeterMode.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "clsSettings.h"
#include "cls6440MeterMode.h"
#include <QJsonDocument>
#include <QLayout>
#include "clsMeterModeFactory.h"
#include "clsRuningSettings.h"
#include "clsSetTestStep.h"
#include <QFileDialog>
#include "clsMeterModeSettings.h"
#include <QMessageBox>
#include "clsShowReport.h"
clsMeterMode::clsMeterMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    lblConnectionType->setText(clsRS::getInst().getConnectionType() + QObject::tr("连接"));
    lblTime->showTime();
    lblStatus->setStatus(IDEL);

    skWidget->setCurrentIndex(0);
    meter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);

    result.setMeter(meter); //用于结果报表显示

    connect(meter,SIGNAL(detectInProgress(QString)),this,SLOT(showMessage(QString)));
    meter->updateGPIB();
    adu200 = new clsSignalThread(this);

    initTable();
    count.reset();
    readSettings();
    updateMessage();

}

void clsMeterMode::showMessage(QString value)
{
    this->lblInfo->showMessage(value,1000);
}


void clsMeterMode::on_btnSetStep_clicked()
{
    clsSetTestStep *dlg = new clsSetTestStep(this);
    dlg->setWindowTitle(tr("设置测试步骤"));

    QStringList tmp;
    for(int i=0; i<steps.length();i++)
    {
        tmp.append( steps.at(i)->getConditon());
    }

    dlg->setTestSteps(tmp);


    if(dlg->exec()==QDialog::Accepted)
    {
        if(tmp == dlg->getTestSteps())
        {
           // qDebug()<<"nothing to do!";
            return;
        }

        steps.clear();
        tmp.clear();


        tmp = dlg->getTestSteps();

        this->result.setSteps(tmp); //用于数据结果显示

        for(int i=0; i< tmp.length();i++)
        {
            WKEMeterMode *tmpMeter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
            tmpMeter->setCondition(tmp.at(i));
            steps.append(tmpMeter);
        }

    }

}

void clsMeterMode::on_btnAdvance_clicked()
{
    clsMeterModeSettings *dlg = new clsMeterModeSettings(this);

    dlg->setCondition(this->mSettings);
    dlg->setStartNumber(this->count.getTotle().toInt());
    dlg->setSingleRes(this->blSingleDisplay);
    if(  dlg->exec())
    {
        mSettings = dlg->getCondtion();
        this->blSingleDisplay = dlg->getSingleRes();
        this->count.totle = dlg->getStartNumber();
        saveSettings();
        updateMessage();
    }
}


void clsMeterMode::on_btnSaveTask_clicked()
{
    QVariantMap task;
    QString strTask;
    task.insert("meterSeres",clsRS::getInst().meterSeries);
    task.insert("stepCount",this->steps.count());

    for(int i=0; i<steps.count();i++)
    {
        task.insert(QString::number(i),steps.at(i)->getConditon());
    }
    QJsonDocument jsonDocument  = QJsonDocument::fromVariant(task);
    if(!jsonDocument.isNull())
        strTask= jsonDocument.toJson(QJsonDocument::Indented);

    QString strTmp;
    strTmp  = QFileDialog::
            getSaveFileName(this,tr("保存测试任务"), tmpDir,
                            tr("WKE FactoryView 任务 (*.wket)"),0);

    if(strTmp.isEmpty())
        return;
    else
    {
        strTaskFile = strTmp;
        tmpDir = QFileInfo(strTaskFile).absoluteDir().path();
        qDebug()<<tmpDir;
        txtTaskFile->setText(QFileInfo(strTaskFile).fileName());
    }

    QFile file(strTaskFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out <<  strTask;

    out.flush();
    file.close();
}

void clsMeterMode::on_btnOpenTask_clicked()
{
    QString strTmp;
    strTmp  = QFileDialog::
            getOpenFileName(this,tr("打开测试任务"), tmpDir,
                            tr("WKE FactoryView 任务 (*.wket)"),0);

    if(strTmp.isEmpty())
        return;
    else
    {
        strTaskFile = strTmp;
        tmpDir = QFileInfo(strTaskFile).absoluteDir().path();
        txtTaskFile->setText(QFileInfo(strTaskFile).fileName());
        count.reset();
    }


    QFile file(strTaskFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString strTask = in.readAll();
    file.close();

    QJsonParseError error;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(strTask.toUtf8(),&error);

    if(error.error == QJsonParseError::NoError)
    {
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty()))
        {
            if(jsonDocument.isObject())
            {
                QVariantMap result = jsonDocument.toVariant().toMap();

                if(result["meterSeres"].toString() == clsRS::getInst().meterSeries)
                {
                    int stepCount = result["stepCount"].toInt();
                    steps.clear();
                    QStringList stepsCondition;
                    stepsCondition.clear();
                    for(int i=0; i<stepCount; i++)
                    {

                        QString tmp = result[QString::number(i)].toString();
                        stepsCondition.append(tmp); //保存所量测的步骤字符串
                        WKEMeterMode * tmpMeter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
                        tmpMeter->setCondition(tmp);
                        steps.append(tmpMeter);
                    }

                    this->result.setSteps(stepsCondition);
                }
                else
                {
                    QMessageBox::warning(this,this->windowTitle(),tr("打开的任务不是这个仪表能够测试的任务！"));
                    return;
                }
            }
        }
    }
}

void clsMeterMode::on_btnTrig_clicked()
{
    trig();
}

void clsMeterMode::initTable()
{
    tabResult->verticalHeader()->setVisible(false);
    tabResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabResult->setSelectionMode(QAbstractItemView::SingleSelection);
    tabResult->setSelectionBehavior(QTableView::SelectRows);
    //    this->tabResult->setColumnCount(2);
    //    this->tabResult->setColumnWidth(0,50);
    //    this->tabResult->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    QStringList headers;
    headers <<tr("序号")<<tr("项目")<<tr("下限")<<tr("测试值")<<tr("上限")<<tr("判定")<<tr("描述");
    tabResult->setColumnCount(headers.length());
    this->tabResult->setColumnWidth(0,60);
    for(int i=0; i< headers.length();i++)
    {
        tabResult->setHorizontalHeaderItem(i,getTableTitleItem(headers.at(i)));

        if(i==0)
            tabResult->setColumnWidth(i,100);
        else if(i==1)
            tabResult->setColumnWidth(i,120);
        else if(i<=5 && i>=2)
            tabResult->setColumnWidth(i,200);
        else
            tabResult->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    }

}
/*!
 * \brief clsSetTestStep::getTableItem 获取单元格
 * \param content 要显示的内容
 * \param isTitle 是否是标题，是标题有加粗效果
 * \return 返回一个item 指针
 */
QTableWidgetItem* clsMeterMode::getTableTitleItem(const QString &content)
{
    QTableWidgetItem * item= new QTableWidgetItem();

    item->setText(content);

    item->setFont(QFont("楷体", 12/*, QFont::Bold*/));

    item->setBackgroundColor(QColor(Qt::gray));
    //item->setTextAlignment(Qt::AlignHCenter/*|Qt::AlignVCenter*/);
    return item;
}


QTableWidgetItem* clsMeterMode::getTableTestItem(const QString &content,int color)
{
    QTableWidgetItem * item= new QTableWidgetItem();

    item->setText(content);

    item->setFont(QFont("楷体", 12/*, QFont::Bold*/));

    switch (color) {
    case 0:
        item->setBackgroundColor(QColor(Qt::red));
        break;
    case 1:
        item->setBackgroundColor(QColor(Qt::green));
        break;

    default:
        break;
    }

    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    return item;
}




void clsMeterMode::trig()
{
    int totleRow=0 ;
    bool status=true;
    lblStatus->setStatus(BUSY);

    QStringList strSaveRes; // 用于保存到硬盘的数据变量

    TESTDATA_STRUCT allStepData; //用于报表数据




    if(blSingleDisplay)
    {
        tabResult->clear();
        tabResult->setRowCount(0);
        this->initTable();
    }

    strSaveRes.append(count.getTotle());
    allStepData.number = count.getTotle().toInt();
    for(int i =0; i< steps.length(); i++)
    {
        totleRow+= steps.at(i)->getCountTestItems();

        meter->setCondition(steps.at(i)->getConditon());
        meter->updateGPIB();    //更新GPIB指令
        meter->repetiveTrig(); //触发仪器，获取测试结果
        QVector<double> singleStepData;
        for(int j=0; j< meter->getCountTestItems(); j++)
        {
            tabResult->setRowCount(tabResult->rowCount()+1);

            //显示序号
            QString number;
            number = QString("%1-%2").arg(count.getTotle()).arg(i+1);
            tabResult->setItem(tabResult->rowCount()-1,0,getTableTestItem(number,2));
            //显示测试项目
            tabResult->setItem(tabResult->rowCount()-1,1,getTableTestItem(meter->getItem(j),2));
            strSaveRes.append(meter->getItem(j));
            //显示下限
            clsMeterLimit tmp = meter->getLimit(j);
            double lowLimit = tmp.getAbsLimitLow();
            QString suffix = meter->getSuffix(j);

            doubleType dt;
            dt.setData(lowLimit);
            tabResult->setItem(tabResult->rowCount()-1,2,getTableTestItem(dt.formateWithUnit(suffix,7)+suffix+UserfulFunctions::getSuffix(meter->getItem(j)),2));

            //显示测试值
            double dblRes = meter->getResult(j);
            singleStepData.append(dblRes); //用于报表显示保存数据
            dt.setData(dblRes);
            tabResult->setItem(tabResult->rowCount()-1,3,getTableTestItem(dt.formateWithUnit(suffix,7)+suffix+UserfulFunctions::getSuffix(meter->getItem(j)),2));
            strSaveRes.append(dt.formateWithUnit(suffix,7)); // 用于保存数据到硬盘
            strSaveRes.append(suffix+UserfulFunctions::getSuffix(meter->getItem(j)));

            //显示上限
            double hiLimit = tmp.getAbsLimitHigh();
            dt.setData(hiLimit);
            tabResult->setItem(tabResult->rowCount()-1,4,getTableTestItem(dt.formateWithUnit(suffix,7)+suffix+UserfulFunctions::getSuffix(meter->getItem(j)),2));

            //显示判定
            QString type;
            bool isPass= tmp.comparaValue(dblRes,type);
            tabResult->setItem(tabResult->rowCount()-1,5,getTableTestItem(type,isPass?1:0));
            strSaveRes.append(type);

            status = status && isPass;
            //  tabResult->setItem(tabResult->rowCount()-1,6,getTableTestItem(meter->getDescription(),2));



        }

        allStepData.data.append(singleStepData);

        //显示描述
        if(meter->getCountTestItems()>=1)
        {
            if(meter->getCountTestItems()>1)
                tabResult->setSpan(tabResult->rowCount()-meter->getCountTestItems(),6,meter->getCountTestItems(),1);
            tabResult->setItem(tabResult->rowCount()-meter->getCountTestItems(),6,getTableTestItem(meter->getDescription(),2));

        }
        // UserfulFunctions::sleepMs(2000);
        //显示当前测试行
        if(tabResult->rowCount()>=1)
        {
            tabResult->setCurrentCell(tabResult->rowCount()-1,0);
            tabResult->showColumn(tabResult->rowCount()-1);
        }

    }

     result.addTestData(allStepData); //用于报表数据
    //整个步骤的判定
    for(int i=0; i< totleRow;i++)
    {
        QTableWidgetItem *item = tabResult->item(tabResult->rowCount()-1-i,0);
        item->setBackgroundColor(status? QColor(Qt::green):QColor(Qt::red));
    }


    lblStatus->setStatus(status);

    strSaveRes.append(status?tr("PASS"):tr("FAIL"));

    //更新测试的数量 pass fail的数据
    if(steps.count()>0)
    {
        count.increase(status);
        txtTestedNumber->setText(QString::number(count.getTotle().toInt()-1));
        txtPassNumber->setText(count.getPass());
        txtFailNumber->setText(count.getFail());


        if(mSettings.saveResType==AllRes)
            saveDataFile(strSaveRes.join(","));
        else if((mSettings.saveResType==PassRes) && status)
            saveDataFile(strSaveRes.join(","));
        else if((mSettings.saveResType==FailRes) && (!status))
            saveDataFile(strSaveRes.join(","));
        else
        {}
    }

    //关闭Bias
    meter->turnOffBias();
}

void clsMeterMode::saveDataFile(QString value)
{
    if(strDataFile.isEmpty())
        return;


    QFile file(strDataFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
        return;


    QTextStream out(&file);
    out.setCodec("GBK");
    out << value << "\n";

    out.flush();
    file.close();

}

void clsMeterMode::on_btnCalibration_clicked()
{
    meter->calibration();
}

void clsMeterMode::on_btnRep_clicked()
{
    while(btnRep->isChecked())
    {
        // detecInprogress();
        if(!btnRep->isChecked())
            return;

        trig();
        UserfulFunctions::sleepMs(50);
        qApp->processEvents();
    }
}

void clsMeterMode::on_btnStop_clicked()
{
    meter->stopDetect();
    btnRep->setChecked(false);
    adu200->stop();

    isStop=false;
    UserfulFunctions::sleepMs(500);
    lblInfo->showMessage("");
    qApp->processEvents();
}

void clsMeterMode::on_btnNewTask_clicked()
{
    this->strTaskFile="";
    this->steps.clear();

    this->txtTaskFile->setText(tr("None"));
    btnSetStep->click();
    count.reset();
}

void clsMeterMode::detecInprogress()
{
    static int i=0;
    int x=i%4+1;
    QIcon icon(QString(":/Icons/rot%1.png").arg(x));
    btnRep->setIcon(icon);
    i++;
}

void clsMeterMode::on_btnSaveData_clicked()
{
    QString strTmp  = QFileDialog::getSaveFileName(this,tr("保存测试数据"), tmpDir,tr("CSV逗号分割文件(*.csv)"),0,QFileDialog::DontConfirmOverwrite);

    if(strTmp.isEmpty())
        return;

    strDataFile = strTmp;
    tmpDir = QFileInfo(strDataFile).absoluteDir().path();
    txtDataFile->setText(QFileInfo(strDataFile).fileName());
}


void  clsMeterMode::updateMessage()
{
    switch (mSettings.saveResType)
    {
    case  AllRes:
        txtSaveType->setText(tr("全部"));
        break;
    case PassRes:
        txtSaveType->setText(tr("通过"));
        break;
    case FailRes:
        txtSaveType->setText(tr("失败"));
        break;
    default:
        break;
    }

    switch (mSettings.trigMode) {
    case SWTrig:
        lblTrigType->setText(tr("软件触发"));
        btnRep->setVisible(false);
        btnTrig->setVisible(true);
        btnStartDetect->setVisible(false);
        btnStop->setVisible(false);
        disconnect(this->adu200,SIGNAL(trigCaptured()),this,SLOT(trig()));
        break;
    case Adu200Trig:
        btnRep->setVisible(false);
        lblTrigType->setText(tr("ADU200触发"));
        adu200->start();
        connect(this->adu200,SIGNAL(trigCaptured()),this,SLOT(trig()));
        connect(this->lblStatus,SIGNAL(statusChange(Status)),this,SLOT(setAdu200(Status)));
        btnStartDetect->setVisible(false);
        btnTrig->setVisible(false);
        btnStop->setVisible(true);
        break;
    case AutoDetectTrig:
        btnTrig->setVisible(false);
        lblTrigType->setText(tr("自动探测样品触发"));
        btnStartDetect->setVisible(true);
        btnRep->setVisible(false);
        btnStop->setVisible(true);
        disconnect(this->adu200,SIGNAL(trigCaptured()),this,SLOT(trig()));
        break;
    default:
        break;
    }
}

void clsMeterMode::setAdu200(Status value)
{
    if(mSettings.trigMode!=Adu200Trig)
        return;

    switch (value) {
    case BUSY:
        adu200->setBDA();
        break;
    case PASS:
        adu200->resetBDA();
        adu200->setPass();
    case FAIL:
        adu200->resetBDA();
        adu200->setFail();
    default:
        break;
    }
}

void clsMeterMode::closeEvent(QCloseEvent *)
{
    btnStop->click();

    if(mSettings.trigMode==Adu200Trig)
    {
        adu200->stop();
    }
}

void clsMeterMode::readSettings()
{
    clsSettings settings;
    QString strNode ="MeterMode/";

    settings.readSetting(strNode+"PreDelay",mSettings.preDelay);
    settings.readSetting(strNode+"lastDelay",mSettings.lastDelay);
    int tmp;
    settings.readSetting(strNode+"trigMode",tmp);
    mSettings.trigMode=(TrigMode)tmp;
    settings.readSetting(strNode+"saveType",tmp);
    mSettings.saveResType=(SaveResultType)tmp;
    settings.readSetting(strNode+"tmpDir",tmpDir);
    settings.readSetting(strNode+"singleDisplay",this->blSingleDisplay);
}

void clsMeterMode::saveSettings()
{
    clsSettings settings;
    QString strNode ="MeterMode/";

    settings.writeSetting(strNode+"PreDelay",mSettings.preDelay);
    settings.writeSetting(strNode+"lastDelay",mSettings.lastDelay);
    settings.writeSetting(strNode+"trigMode",mSettings.trigMode);
    settings.writeSetting(strNode+"saveType",mSettings.saveResType);
    settings.writeSetting(strNode+"tmpDir",tmpDir);
    settings.writeSetting(strNode+"singleDisplay",this->blSingleDisplay);

}


void clsMeterMode::on_btnStartDetect_clicked()
{
    isStop=true;
    while(isStop)
    {
        UserfulFunctions::sleepMs(mSettings.preDelay);

        if(meter->detectDut())
        {
            UserfulFunctions::sleepMs(mSettings.lastDelay);
            trig();
        }
        else
            goto Stop;

        qApp->processEvents();
    }

Stop:
    btnStartDetect->setChecked(false);
    meter->turnOffBias();
}

void clsMeterMode::on_btnReport_clicked()
{
    clsShowReport *dlg = new clsShowReport(this);
    dlg->setData((clsTestResult*)(&this->result));

    dlg->exec();
}

void clsMeterMode::on_btnRep10_clicked()
{
    for(int i=0; i< 50; i++)
    {
        trig();
    }
}
