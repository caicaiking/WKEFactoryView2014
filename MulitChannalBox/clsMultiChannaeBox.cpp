#include "clsSettings.h"
#include "clsMultiChannaeBox.h"
#include <math.h>
#include <QTime>
#include "clsSwitchBoxTest.h"
#include "clsCalibration.h"
#include "clsMultModeMeterUi.h"
#include <QFile>
#include <QDir>
#include <Quazip/JlCompress.h>
#include <QFileDialog>
#include "clsChennalSelect.h"
#include <QTime>
#include "UserfulFunctions.h"
#include <QTextStream>
#include "clsCalibrationDbOp.h"
#include "cls6440MultiMeterMode.h"
#include "clsMultiChannelSettings.h"
clsMultiChannaeBox::clsMultiChannaeBox(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    commands=initCommand();

    bool initCom= clsConnectSWBox::Instance()->initSerialPort();
    btnOpen->setEnabled(!initCom);
    btnOpen->setVisible(!initCom);
    btnSwitchBoxTest->setEnabled(initCom);

    meter = new  cls6440MultiMeterMode();

    channels="1";
    tableWidget->setColumnCount(4);
    int i= CHENNAL_COUNT / 4;
    if(i*4<CHENNAL_COUNT)
        i++;
    tableWidget->setRowCount(i);
    tableWidget->horizontalHeader()->setAutoScroll(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    initPannel();
    initDataBase();
    readSettings();
    this->showMaximized();
}

/*!
 * \brief  初始化数据库文件。
 */
void clsMultiChannaeBox::initDataBase()
{
    clsCalDb::getInst()->setStrDataBaseName(QString("./McbCal.db"));
    clsCalDb::getInst()->openDataBase();
    clsCalDb::getInst()->initTable();
}

void clsMultiChannaeBox::readSettings()
{
    clsSettings settings;
    QString strNode ="MulitChannel/";
    settings.readSetting(strNode +"SwitchDelay",this->switchDelay);
    switchDelay =(switchDelay==0?15:switchDelay);
    settings.readSetting(strNode+"IsUseLoadValue",this->isUseLoadValue);
}

void clsMultiChannaeBox::writeSettings()
{
    clsSettings settings;
    QString strNode ="MulitChannel/";
    settings.writeSetting(strNode +"SwitchDelay",this->switchDelay);
    settings.writeSetting(strNode+"IsUseLoadValue",this->isUseLoadValue);
}

/*!
 * \brief clsMultiChannaeBox::initCommand
 * \return 通道切换列表
 * 初始化命令
 */
QStringList clsMultiChannaeBox::initCommand()
{
    QStringList commands;
    commands.clear();
    QString tmp;

    for(int i=0; i<16; i++)
    {
        int value=  pow(2,i);
        tmp = QString("4,%1,0").arg(QString::number(value));
        commands<<tmp;
    }

    for(int i=1; i<5; i++)
    {
        int value=  pow(2,i-1);
        tmp = QString("4,0,%1").arg(QString::number(value));
        commands<<tmp;
    }

    return commands;
}

/*!
 * \brief clsMultiChannaeBox::on_btnOpen_clicked
 * 打开多通道
 */
void clsMultiChannaeBox::on_btnOpen_clicked()
{
    bool isInit = clsConnectSWBox::Instance()->initSerialPort();

    btnOpen->setEnabled(!isInit);
    btnSwitchBoxTest->setEnabled(isInit);
}

/*!
 * \brief clsMultiChannaeBox::on_btnSwitchBoxTest_clicked
 * 多通道控制箱测试
 */
void clsMultiChannaeBox::on_btnSwitchBoxTest_clicked()
{
    clsSwitchBoxTest sw(this);
    sw.exec();
}


/*!
 * \brief clsMultiChannaeBox::on_btnMeter_clicked
 * 仪表单次测试
 */
void clsMultiChannaeBox::on_btnMeter_clicked()
{
    clsMultiModeMeterUi * dlg = new clsMultiModeMeterUi();

    dlg->setConditon(this->meter->getConditon());

    if(dlg->exec() == QDialog::Accepted)
    {
        this->meter->setCondition(dlg->getConditon());
    }

}

/*!
 * \brief clsMultiChannaeBox::on_btnSave_clicked
 * 保存设定
 */
void clsMultiChannaeBox::on_btnSave_clicked()
{
    if(strSaveFileName.isEmpty())
        strSaveFileName = "untitled.wkm";

    QString fileName = QFileDialog::getSaveFileName(this, tr("保存多通道测试文件"),
                                                    strSaveFileName,
                                                    tr("多通道测试文件 (*.wkm)"));
    strSaveFileName= fileName;

    if(strSaveFileName.isEmpty())
        return;

    QDir dir("./");
    if (!dir.exists("tmp"))
    {
        dir.mkdir("tmp");
    }

    QFile file("./tmp/Test1.txt");

    if(file.open(QFile::WriteOnly))
    {
        QVariantMap tmpVar;
        tmpVar.insert("Meter",meter->getConditon());
        tmpVar.insert("Chennal",this->channels);

        QJsonDocument jsDocument = QJsonDocument::fromVariant(tmpVar);
        if(jsDocument.isObject())
            file.write(jsDocument.toJson());
        file.close();
    }

    QDir dirFile("./tmp");
    QStringList list=dirFile.entryList(QStringList()<<"*.txt",QDir::Files);

    QStringList files;
    foreach (QString item, list) {
        QString a ="./tmp/"+ item;
        files.append(a);
    }

    //打包文件
    JlCompress::compressFiles(fileName,files);

    //删除所有生成的文件
    foreach (QString item , files) {
        dir.remove(item);
    }

}


void clsMultiChannaeBox::  on_btnOpenSettingFile_clicked()
{

    if(strSaveFileName.isEmpty())
    {
        strSaveFileName = "untitled.wkm";
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("保存多通道测试文件"),
                                                    strSaveFileName,
                                                    tr("多通道测试文件 (*.wkm)"));
    if(fileName.isEmpty())
        return;

    strSaveFileName = fileName;

    QDir dir("./");
    if (!dir.exists("tmp"))
    {
        dir.mkdir("tmp");
    }

    JlCompress::extractDir(fileName,"./tmp");

    QDir dirFile("./tmp");
    QStringList list=dirFile.entryList(QStringList()<<"*.txt",QDir::Files);

    if(!list.contains("Test1.txt"))
        return;

    QFile file("./tmp/Test1.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString strRead = in.readAll();

        //qDebug()<< strRead;

        QJsonParseError error;
        QJsonDocument jsDocomnent = QJsonDocument::fromJson(strRead.toUtf8(),&error);
        if(error.error==QJsonParseError::NoError)
        {
            if(jsDocomnent.isObject())
            {
                QVariantMap result = jsDocomnent.toVariant().toMap();
                this->channels = result["Chennal"].toString();
                QString stConditon = result["Meter"].toString();
                this->meter->setCondition(stConditon);
            }
        }

    }

    QStringList files;
    foreach (QString item, list) {
        QString a ="./tmp/"+ item;
        files.append(a);
    }

    //删除所有生成的文件
    foreach (QString item , files) {
        dir.remove(item);
    }

    initPannel();

    //  qDebug()<<fileName;
}

void clsMultiChannaeBox::on_btnSignleTest_clicked()
{
    btnSignleTest->setEnabled(false);
    setIdeal();
    for(int i=0; i< pannel.length();i++)
    {
        clsConnectSWBox::Instance()->sendCommand(pannel.at(i)->number()-1);
        UserfulFunctions::sleepMs(switchDelay);
        meter->setChannel(pannel.at(i)->number());
        meter->setUseLoad(isUseLoadValue); //以后再说
        meter->trig();

        TestResult res;

        for(int j=0; j< meter->getTotleItemCount();j++)
        {
            TestItem testItem;
            testItem.item = meter->getItem(j);
            testItem.limit =meter->getLimit(j);
            testItem.status = meter->getStatus(j);
            testItem.suffix = meter->getUnit(j);
            testItem.value = meter->getRes(j);

            res.item.append(testItem);
        }
        res.status = meter->getTotleStatus();
        pannel.at(i)->setTestResult(res);
    }
    btnSignleTest->setEnabled(true);

}

void clsMultiChannaeBox::setIdeal()
{
    for(int i=0; i< pannel.length();i++)
        pannel.at(i)->clearAll();
}

void clsMultiChannaeBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() ==Qt::Key_Return)
    {
        btnSignleTest->clicked();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void clsMultiChannaeBox::closeEvent(QCloseEvent *)
{
    clsConnectSWBox::Instance()->closeSeriesPort();
}


void clsMultiChannaeBox::on_btnSelectChennal_clicked()
{
    clsChennalSelect dlg(this);
    dlg.setChennal(this->channels);
    if(dlg.exec())
    {
        this->channels=dlg.getChennal();
        initPannel();
    }
}




void clsMultiChannaeBox::on_btnClearAllData_clicked()
{
    for(int i=0; i< pannel.length();i++)
        pannel.at(i)->clearAll();
}

void clsMultiChannaeBox::initPannel()
{
    int m=0, n=0;
    for(int i=0;i< pannel.length();i++)
    {
        m=i%4;
        n=i/4;

        tableWidget->removeCellWidget(n,m);
    }

    pannel.clear();

    foreach (QString tmp, channels.split(",")) {
        bool ok;
        int value = tmp.toInt(&ok);

        if(ok)
        {
            clsMRBDisplayPannel * tmpWiget = new clsMRBDisplayPannel();
            tmpWiget->setNumber(value);
            pannel.append(tmpWiget);
        }

    }
    m=0;n=0;
    for(int i=0; i< pannel.length(); i++)
    {
        m=i%4;
        n=i/4;
        tableWidget->setCellWidget(n,m,pannel.at(i));
        tableWidget->setRowHeight(n,pannel.at(i)->height());
    }
    btnShowTestStatus->toggled(false);
}

void clsMultiChannaeBox::on_btnShowTestStatus_toggled(bool checked)
{
    if(checked)
    {
        btnShowTestStatus->setText(tr("显示\n数据"));
        for(int i=0; i< pannel.length();i++)
            pannel.at(i)->setResType(0);
    }
    else
    {
        btnShowTestStatus->setText(tr("显示\n状态"));
        for(int i=0; i< pannel.length();i++)
            pannel.at(i)->setResType(1);
    }
}

void clsMultiChannaeBox::on_btnChannalCal_clicked()
{
    clsCalibration *dlg = new clsCalibration(this);
    dlg->setMeter(this->meter);
    dlg->setChannels(this->channels.split(","));
    dlg->exec();
}

void clsMultiChannaeBox::on_btnRunningSettings_clicked()
{
    clsMultiChannelSettings *dlg = new clsMultiChannelSettings(this);

    dlg->setConditon(switchDelay,isUseLoadValue);
    if(dlg->exec()==QDialog::Accepted)
    {
        switchDelay = dlg->getSwitchDelay();
        isUseLoadValue = dlg->isUseLoadData();
        writeSettings();
    }
}
