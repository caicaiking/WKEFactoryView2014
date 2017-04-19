#include "clsMeterMode.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "clsDog.h"
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
#include "clsStatistics.h"
#include "clsShowReport.h"


clsMeterMode::clsMeterMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    btnRep300->setVisible(false);
    btnStepStop->setVisible(false);
    lblConnectionType->setText(clsRS::getInst().getConnectionType() + QObject::tr("连接"));
    lblTime->showTime();
    lblStatus->setStatus(IDEL);

    skWidget->setCurrentIndex(0);
    meter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);

    result.setMeter(meter); //用于结果报表显示

    //连接meter的探测信号到显示信息上面
    connect(meter,SIGNAL(detectInProgress(QString)),this,SLOT(showMessage(QString)));
    meter->updateGPIB();
    adu200 = new clsSignalThread(this);

    initTable();
    count.reset();
    readSettings();
    updateMessage();
    this->showMaximized();


    setDemoVersion(SingletonDog::Instance()->getVersion());

    initLog();
}

void clsMeterMode:: setDemoVersion(bool value)
{
    this->btnOpenTask->setEnabled(value);
    this->btnSaveTask->setEnabled(value);
    this->btnSaveData->setEnabled(value);
    this->btnReport->setEnabled(value);
    this->btnExportData->setEnabled(value);
}

//初始化Log记录
void clsMeterMode::initLog()
{
    passDataLog = new clsLog(this);
    passDataLog->setFileName("pass.log");

    failDataLog = new clsLog(this);
    failDataLog->setFileName("fail.log");

    allDataLog = new clsLog(this);
    allDataLog->setFileName("all.log");

    connect(passDataLog,SIGNAL(showErrorMessage(QString)),this,SLOT(showLogError(QString)));
    connect(failDataLog,SIGNAL(showErrorMessage(QString)),this,SLOT(showLogError(QString)));
    connect(allDataLog,SIGNAL(showErrorMessage(QString)),this,SLOT(showLogError(QString)));

    passDataLog->run();
    failDataLog->run();
    allDataLog->run();
}


//显示软件运行信息
void clsMeterMode::showMessage(QString value)
{
    this->lblInfo->showMessage(value,1000);
}

//设置测试程序
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
        clearLogFile();

        count.fail=0;
        count.pass=0;
        count.totle=1;

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

//用于软件设定
void clsMeterMode::on_btnAdvance_clicked()
{
    clsMeterModeSettings *dlg = new clsMeterModeSettings(this);

    this->mSettings.startNumber = this->count.getTotle().toInt();
    dlg->setCondition(this->mSettings);

    if(dlg->exec())
    {
        mSettings = dlg->getCondtion();
        saveSettings();
        updateMessage();
    }
}

//保存测试任务
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
        // qDebug()<<tmpDir;
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


void clsMeterMode::clearLogFile()
{
    allDataLog->clearFile();
    passDataLog->clearFile();
    failDataLog->clearFile();
}

//读取测试任务
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

    clearLogFile();
}

//用于软件触发
void clsMeterMode::on_btnTrig_clicked()
{
    btnTrig->setEnabled(false);
    qApp->processEvents();
    trig();
    qApp->processEvents();
    btnTrig->setEnabled(true);
}


//初始化工作表
void clsMeterMode::initTable()
{
    tabResult->verticalHeader()->setVisible(false);
    tabResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //  tabResult->setSelectionMode(QAbstractItemView::SingleSelection);
    //  tabResult->setSelectionBehavior(QTableView::SelectRows);


    //    this->tabResult->setColumnCount(2);
    //    this->tabResult->setColumnWidth(0,50);
    //    this->tabResult->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    QStringList headers;
    headers <<tr("序号")<<tr("项目")<<tr("测试条件")<<tr("下限")<<tr("测试值")<<tr("上限")<<tr("判定")<<tr("描述");
    tabResult->setColumnCount(headers.length());
    this->tabResult->setColumnWidth(0,80);
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

//生成单个表格单元格内容
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

//单次测试
void clsMeterMode::trig()
{
    if(!checkDog())
        return;

    //为了避免一个测试没有完成又进行下一次测试，做了一下修改
    if(lblStatus->getStatus()== BUSY)
        return;

    bool blRetest = false;
RETEST:
    int totleRow=0 ;
    bool status=true;
    stepStop = false;
    btnStepStop->setVisible(btnTrig->isVisible());

    if(blRetest)
    {
        lblInfo->showMessage(tr("已经开始第二次测试"),2);
    }



    lblStatus->setStatus(BUSY);

    QStringList strSaveRes; // 用于保存到硬盘的数据变量

    TESTDATA_STRUCT allStepData; //用于报表数据



    //是否是单次显示
    if(this->mSettings.displayResultType)
    {
        tabResult->clear();
        tabResult->setRowCount(0);
        this->initTable();
    }

    //写入文件的结果序号
    strSaveRes.append(count.getTotle());
    //所有的测试结果的总计
    allStepData.number = count.getTotle().toInt();
    for(int i =0; i< steps.length(); i++)
    {
        if(stepStop == true)
        {
            lblStatus->setStatus(IDEL);
            return;
        }

        totleRow+= steps.at(i)->getCountTestItems();

        meter->setCondition(steps.at(i)->getConditon());
        meter->updateGPIB();    //更新GPIB指令
        //  UserfulFunctions::sleepMs(20);
        meter->repetiveTrig(); //触发仪器，获取测试结果
        QVector<double> singleStepData;
        for(int j=0; j< meter->getCountTestItems(); j++)
        {
            qApp->processEvents();
            tabResult->setRowCount(tabResult->rowCount()+1);

            //显示序号
            QString number;
            number = QString("%1-%2").arg(count.getTotle()).arg(i+1);
            tabResult->setItem(tabResult->rowCount()-1,NoCol,getTableTestItem(number,2));
            //显示测试项目
            tabResult->setItem(tabResult->rowCount()-1,ItemCol,getTableTestItem(meter->getItem(j),2));
            strSaveRes.append(meter->getItem(j));

            //显示测试条件
            QString testCon = meter->getFreq()+" "+meter->getLevel()+" "+meter->getBias();
            tabResult->setItem(tabResult->rowCount()-1,ConCol,getTableTestItem(testCon,2));
            //显示下限
            clsMeterLimit tmp = meter->getLimit(j);
            double lowLimit = tmp.getAbsLimitLow();
            QString suffix = meter->getSuffix(j);

            doubleType dt;
            dt.setData(lowLimit);
            tabResult->setItem(tabResult->rowCount()-1,LowCol,getTableTestItem(dt.formateWithUnit(suffix,7)+suffix+UserfulFunctions::getSuffix(meter->getItem(j)),2));

            //显示测试值
            double dblRes = meter->getResult(j);
            singleStepData.append(dblRes); //用于报表显示保存数据
            dt.setData(dblRes);
            tabResult->setItem(tabResult->rowCount()-1,ResCol,getTableTestItem(dt.formateWithUnit(suffix,7)+suffix+UserfulFunctions::getSuffix(meter->getItem(j)),2));
            strSaveRes.append(dt.formateWithUnit(suffix,7)); // 用于保存数据到硬盘
            strSaveRes.append(suffix+UserfulFunctions::getSuffix(meter->getItem(j)));

            //显示上限
            double hiLimit = tmp.getAbsLimitHigh();
            dt.setData(hiLimit);
            tabResult->setItem(tabResult->rowCount()-1,HiCol,getTableTestItem(dt.formateWithUnit(suffix,7)+suffix+UserfulFunctions::getSuffix(meter->getItem(j)),2));

            //显示判定
            QString type;
            bool isPass= tmp.comparaValue(dblRes,type);
            tabResult->setItem(tabResult->rowCount()-1,StaCol,getTableTestItem(type,isPass?1:0));
            strSaveRes.append(type);

            status = status && isPass;
            //  tabResult->setItem(tabResult->rowCount()-1,6,getTableTestItem(meter->getDescription(),2));
            qApp->processEvents();
            //在这儿进行单步的是否通过做一个判断
            if(mSettings.failPass && (!status))
            {
                if(mSettings.failRetestOnce==false)
                    goto PASSTEST;
                else
                {
                    if(blRetest==true)
                        goto PASSTEST;
                    else
                    {
                        blRetest = true;
                        goto RETEST;
                    }
                }
            }

        }

        allStepData.data.append(singleStepData);

        //显示描述
        if(meter->getCountTestItems()>=1)
        {
            if(meter->getCountTestItems()>1)
                tabResult->setSpan(tabResult->rowCount()-meter->getCountTestItems(),DescCol,meter->getCountTestItems(),1);
            tabResult->setItem(tabResult->rowCount()-meter->getCountTestItems(),DescCol,getTableTestItem(meter->getDescription(),2));

        }
        // UserfulFunctions::sleepMs(2000);
        //显示当前测试行
        if(tabResult->rowCount()>=1)
        {
            tabResult->setCurrentCell(tabResult->rowCount()-1,0);
            tabResult->showColumn(tabResult->rowCount()-1);
        }
    }

    //result.addTestData(allStepData); //用于报表数据
    //整个步骤的判定
    for(int i=0; i< totleRow;i++)
    {
        QTableWidgetItem *item = tabResult->item(tabResult->rowCount()-1-i,0);
        item->setBackgroundColor(status? QColor(Qt::green):QColor(Qt::red));
    }

    strSaveRes.append(status?tr("PASS"):tr("FAIL"));

    //更新测试的数量 pass fail的数据
    if(steps.count()>0)
    {
        count.increase(status);
        txtTestedNumber->setText(QString::number(count.getTotle().toInt()-1));
        txtPassNumber->setText(count.getPass());
        txtFailNumber->setText(count.getFail());

        allDataLog->writeLine(strSaveRes.join(this->mSettings.sp)); //数据写入log文件
        if(status)
            passDataLog->writeLine(strSaveRes.join(this->mSettings.sp)); //数据写入log文件
        else
            failDataLog->writeLine(strSaveRes.join(mSettings.sp)); //数据写入log文件

        if(mSettings.saveResType==AllRes)
        {
            result.addTestData(allStepData); //用于报表数据
            saveDataFile(strSaveRes.join(mSettings.sp));
        }
        else if((mSettings.saveResType==PassRes) && status)
        {
            result.addTestData(allStepData); //用于报表数据
            saveDataFile(strSaveRes.join(mSettings.sp));
        }
        else if((mSettings.saveResType==FailRes) && (!status))
        {
            result.addTestData(allStepData); //用于报表数据
            saveDataFile(strSaveRes.join(mSettings.sp));
        }
        else
        {/*Donthing here!*/}
    }

PASSTEST:
    lblStatus->setStatus(status);

    //关闭Bias
    meter->turnOffBias();
    btnStepStop->setVisible(false);
}

void clsMeterMode::showLogError(QString value)
{
    QMessageBox::warning(this,tr("警告"),value);
}

bool clsMeterMode::checkDog()
{
    QString strProductName;

    if(!SingletonDog::Instance()->getVersion())
    {}
    else
    {
        if(((!SingletonDog::Instance()->getName(strProductName))|| (strProductName !="WKE FactoryView 2014")))
        {
            QMessageBox::warning(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("请插入加密狗！"));
            return false;
        }
    }
    return true;
}

//保存测试数据文件
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

//仪表校准
void clsMeterMode::on_btnCalibration_clicked()
{
    meter->calibration();
}

//连续测试 没有实际的用处，用于软件测试
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

//停止一切测试连接
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

//新的测试任务
void clsMeterMode::on_btnNewTask_clicked()
{
    this->strTaskFile="";
    this->steps.clear();

    this->txtTaskFile->setText(tr("None"));
    btnSetStep->click();
    count.reset();
}

//显示在探测样品 但是没有用
void clsMeterMode::detecInprogress()
{
    static int i=0;
    int x=i%4+1;
    QIcon icon(QString(":/Icons/rot%1.png").arg(x));
    btnRep->setIcon(icon);
    i++;
}

//保存数据
void clsMeterMode::on_btnSaveData_clicked()
{
    QString strTmp  = QFileDialog::getSaveFileName(this,tr("保存测试数据"), tmpDir,tr("CSV逗号分割文件(*.csv)"),0,QFileDialog::DontConfirmOverwrite);

    if(strTmp.isEmpty())
        return;

    strDataFile = strTmp;
    tmpDir = QFileInfo(strDataFile).absoluteDir().path();
    txtDataFile->setText(QFileInfo(strDataFile).fileName());
}

//更新消息并且连接测试方式
void  clsMeterMode::updateMessage()
{
    showMessage("");
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
        disconnect(this->adu200,SIGNAL(showStatus(QString)),this,SLOT(showMessage(QString)));
        break;
    case Adu200Trig:
        btnRep->setVisible(false);
        lblTrigType->setText(tr("ADU200触发"));
        adu200->start();
        disconnect(this->adu200,SIGNAL(trigCaptured()),this,SLOT(adu200Trig()));
        connect(this->adu200,SIGNAL(trigCaptured()),this,SLOT(adu200Trig()));
        disconnect(this->lblStatus,SIGNAL(statusChange(Status)),this,SLOT(setAdu200(Status)));  //| 这两个是用于adu200的信号输出的 |
        connect(this->lblStatus,SIGNAL(statusChange(Status)),this,SLOT(setAdu200(Status)));     //| 如果不惜要这个的话可以将此注释 |
        disconnect(this->adu200,SIGNAL(showStatus(QString)),this,SLOT(showMessage(QString)));
        connect(this->adu200,SIGNAL(showStatus(QString)),this,SLOT(showMessage(QString)));

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
        disconnect(this->adu200,SIGNAL(showStatus(QString)),this,SLOT(showMessage(QString)));
        break;
    default:
        break;
    }
}

//adu200触发测试
void clsMeterMode::adu200Trig()
{
    //双触发的控管
    adu200->stop();
    trig();
    adu200->start();
}

//adu200输出信号
void clsMeterMode::setAdu200(Status value)
{
    if(mSettings.trigMode!=Adu200Trig)
        return;

    switch (value) {
    case BUSY:
        adu200->setBusy();
        break;
    case PASS:
        adu200->setPass();
        adu200->resetBusy();
        adu200->emitBDA();
    case FAIL:

        adu200->setFail();
        adu200->resetBusy();
        adu200->emitBDA();
    default:
        break;
    }
}

//关闭事件处理
void clsMeterMode::closeEvent(QCloseEvent *)
{
    btnStop->click();
    allDataLog->stop();
    passDataLog->stop();
    failDataLog->stop();

    if(mSettings.trigMode==Adu200Trig)
    {
        adu200->stop();
    }
}

void clsMeterMode::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() == Qt::ControlModifier && e->key()==Qt::Key_9)
    {
        btnRep300->setVisible(!btnRep300->isVisible());
    }
}


//读取设定
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
    settings.readSetting(strNode+"singleDisplay",this->mSettings.displayResultType);
    settings.readSetting(strNode+"FailPassDut",this->mSettings.failPass);
    settings.readSetting(strNode+"failRetest",this->mSettings.failRetestOnce);
    settings.readSetting(strNode+"Sp",this->mSettings.sp);

    if(this->mSettings.sp.isEmpty())
    {
        if(QLocale().decimalPoint()==QChar('.'))
            mSettings.sp=",";
        else
            mSettings.sp=";";
    }
}
//保存设定
void clsMeterMode::saveSettings()
{
    clsSettings settings;
    QString strNode ="MeterMode/";

    settings.writeSetting(strNode+"tmpDir",tmpDir);
    settings.writeSetting(strNode+"PreDelay",mSettings.preDelay);
    settings.writeSetting(strNode+"lastDelay",mSettings.lastDelay);
    settings.writeSetting(strNode+"trigMode",mSettings.trigMode);
    settings.writeSetting(strNode+"saveType",mSettings.saveResType);

    settings.writeSetting(strNode+"singleDisplay",this->mSettings.displayResultType);
    settings.writeSetting(strNode+"FailPassDut",this->mSettings.failPass);
    settings.writeSetting(strNode+"failRetest",this->mSettings.failRetestOnce);
    settings.writeSetting(strNode+"Sp",this->mSettings.sp);
}

//开始探测
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
//打印报表
void clsMeterMode::on_btnReport_clicked()
{
    clsShowReport *dlg = new clsShowReport(this);
    dlg->setData(&this->result);


    //    clsStatistics *dlg = new clsStatistics(this);
    //    dlg->setData(&result);

}

void clsMeterMode::on_btnRep300_clicked()
{


    btnRep300->setText(tr("停止\n测试"));



    while(btnRep300->isChecked())
    {
        trig();
        UserfulFunctions::sleepMs(10);
    }
    btnRep300->setText(tr("重复\n测试"));

}

void clsMeterMode::on_btnExportData_clicked()
{

    allDataLog->flushResult();
    passDataLog->flushResult();
    failDataLog->flushResult();

    QString strTmp  = QFileDialog::getSaveFileName(this,tr("保存测试数据"), tmpDir,tr("CSV逗号分割文件(*.csv)"),0,QFileDialog::DontConfirmOverwrite);

    QString myTmpDatafile;
    if(strTmp.isEmpty())
        return;

    myTmpDatafile = strTmp;
    tmpDir = QFileInfo(myTmpDatafile).absoluteDir().path();



    if(mSettings.saveResType==AllRes)
    {
        if(QFile::exists("all.log"))
            QFile::copy("all.log", myTmpDatafile);
        else
            QMessageBox::information(this,tr("消息"),tr("没有数据可以保存"));
    }
    else if((mSettings.saveResType==PassRes))
    {
        if(QFile::exists("pass.log"))
            QFile::copy("pass.log", myTmpDatafile);
        else
            QMessageBox::information(this,tr("消息"),tr("没有数据可以保存"));
    }
    else if((mSettings.saveResType==FailRes))
    {
        if(QFile::exists("fail.log"))
            QFile::copy("fail.log", myTmpDatafile);
        else
            QMessageBox::information(this,tr("消息"),tr("没有数据可以保存"));
    }
}

void clsMeterMode::on_btnStatics_clicked()
{
    clsStatistics *dlg = new clsStatistics(this);
    dlg->setData(&result);
}

void clsMeterMode::on_btnCopy_clicked()
{

    int row = tabResult->rowCount();
    int col = tabResult->columnCount();
    //qDebug()<< row <<" " <<col;
    QString strCp;
    for(int r=0; r< row; r++)
    {

        for(int c=0; c<col; c++)
        {
            QTableWidgetItem * item = tabResult->item(r,c);
            if(item!=NULL && item->isSelected())
            {
                strCp+=tabResult->item(r,c)->text();
                strCp+= "\t";
            }
        }
        strCp += "\n";
    }
    // qDebug()<<strCp;
    QApplication::clipboard()->setText(strCp);
}



void clsMeterMode::on_btnStepStop_clicked()
{
    this->stepStop = true;
    qApp->processEvents();
    btnStepStop->setVisible(false);
}
