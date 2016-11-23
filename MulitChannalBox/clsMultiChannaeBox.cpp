#include <QDir>
#include "clsSettings.h"
#include "frmAbout.h"
#include "clsMultiChannaeBox.h"
#include <math.h>
#include <QTime>
#include "clsSwitchBoxTest.h"
#include "clsCalibration.h"
#include <QColor>
#include "clsMultModeMeterUi.h"
#include <QFile>
#include <QDir>
#include <JlCompress.h>
#include <QFileDialog>
#include "clsChennalSelect.h"
#include <QTime>
#include "UserfulFunctions.h"
#include <QTextStream>
#include "clsCalibrationDbOp.h"
#include "cls6440MultiMeterMode.h"
#include "clsMultiChannelSettings.h"
#include "clsWriteDataToFile.h"
#include "clsMultiChannelMeterFactory.h"
clsMultiChannaeBox::clsMultiChannaeBox(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    commands=initCommand();

    bool initCom= clsConnectSWBox::Instance()->initSerialPort();
    btnOpen->setEnabled(!initCom);
    btnOpen->setVisible(!initCom);
    btnSwitchBoxTest->setEnabled(initCom);

    meter = clsMultiChannelMeterFactory::getMeter(clsRS::getInst().meterSeries);
    meter->setConditionForCalibration(0);

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


    readSettings();

    initPannel();
    initDataBase();
    initSweepMode();

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
    settings.readSetting(strNode + "Channals",this->channels);
    QString tmpMeter;
    settings.readSetting(strNode + "Meter",tmpMeter);
    meter->setCondition(tmpMeter);

    int tmpIndex;
    settings.readSetting(strNode + "DispIndex",tmpIndex);
    dispStactWindow->setCurrentIndex(tmpIndex);

    cmbItem->clear();
    for(int i=0; i< meter->getTotleItemCount();i++)
    {
        cmbItem->addItem(meter->getItem(i));
    }

}

/*!
 * \brief 写入配置文件。
 */
void clsMultiChannaeBox::writeSettings()
{
    clsSettings settings;
    QString strNode ="MulitChannel/";
    settings.writeSetting(strNode +"SwitchDelay",this->switchDelay);
    settings.writeSetting(strNode+"IsUseLoadValue",this->isUseLoadValue);
    settings.writeSetting(strNode +"Channals",this->channels);
    settings.writeSetting(strNode + "Meter",this->meter->getConditon());
    settings.writeSetting(strNode + "DispIndex",this->dispStactWindow->currentIndex());
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
        stop();
        initSweepMode();
        writeSettings();
    }

}

void clsMultiChannaeBox::stop()
{
    btnSignleTest->setChecked(false);
    btnSignleTest->setText(tr("开始\n测试"));
    btnSignleTest->setIcon(QIcon(":/Icons/single.png"));
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

/*!
 * \brief clsMultiChannaeBox::itemTrig
 * \param i
 */
void clsMultiChannaeBox::itemTrig(clsMRBDisplayPannel * value)
{
    clsConnectSWBox::Instance()->sendCommand(value->number()-1);
    UserfulFunctions::sleepMs(switchDelay);
    meter->setChannel(value->number());
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
    value->setTestResult(res);
}

void clsMultiChannaeBox::on_btnSignleTest_clicked()
{
    if(!btnSignleTest->isChecked())
        return;

    btnSignleTest->setText(tr("停止\n测试"));
    btnSignleTest->setIcon(QIcon(":/Icons/stop.png"));

    //产品测试模式
    if(dispStactWindow->currentIndex()==0)
    {
        setIdeal();
        for(int i=0; i< pannel.length();i++)
        {
            itemTrig(pannel.at(i));
            if(!btnSignleTest->isChecked())
            {
                btnSignleTest->setText(tr("开始\n测试"));
                return;
            }
        }
    }
    else //这里面是曲线记录模式
    {
        double count=0;
        plotWidget->clearData(); //清除所有数据
        mkDataDir();
        clsWriteDataToFile file;
        file.setChannel(this->channels);
        file.setPath("./MultiChannelDataFile/");

        QStringList items;
        for(int i=0; i<meter->getTotleItemCount();i++)
        {
            items.append(meter->getItem(i));
        }
        file.writeTitle(items.join(","));


        file.startRecord();
        // qDebug()<<"Mede";
        while(btnSignleTest->isChecked())
        {
            qApp->processEvents();
            QTime timeStart = QTime::currentTime();
            for(int j=0; j<channels.split(",").length(); j++)
            {
                QString ch = channels.split(",").at(j);
                if(!ch.isEmpty())
                {
                    clsConnectSWBox::Instance()->sendCommand(ch.toInt()-1);
                    UserfulFunctions::sleepMs(this->switchDelay);
                    qApp->processEvents();
                    meter->setChannel(ch.toInt());
                    meter->setUseLoad(isUseLoadValue); //以后再说
                    meter->trig();

                    QList<double> tmpRes;
                    for(int xx=0; xx< meter->getTotleItemCount(); xx++)
                    {
                        tmpRes.append(meter->getRes(xx));
                    }
                    file.setData(ch,tmpRes);
                    plotWidget->setData(ch.toInt(),count,tmpRes);
                    qApp->processEvents();
                }
            }
            qApp->processEvents();
            count++;
            UserfulFunctions::sleepMs(txtDelay->value());
            lblCount->setText(QString::number(count));
            QTime timeStop = QTime::currentTime();
            lblTime->setText(QString::number(timeStart.msecsTo(timeStop))+"ms");
        }
        file.stopRecord();
    }
    btnSignleTest->setChecked(false);
    btnSignleTest->setText(tr("开始\n测试"));
    btnSignleTest->setIcon(QIcon(":/Icons/single.png"));

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
        // btnSignleTest->clicked();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void clsMultiChannaeBox::closeEvent(QCloseEvent *)
{
    clsConnectSWBox::Instance()->closeSeriesPort();
    writeSettings();
}

void clsMultiChannaeBox::on_btnSelectChennal_clicked()
{
    clsChennalSelect dlg(this);
    dlg.setChennal(this->channels);
    if(dlg.exec())
    {
        this->channels=dlg.getChennal();
        stop();
        initPannel();
        initSweepMode();
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
        connect(pannel.at(i),SIGNAL(numberClick(clsMRBDisplayPannel*)),this,SLOT(itemClick(clsMRBDisplayPannel*)));
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

void clsMultiChannaeBox::itemClick(clsMRBDisplayPannel * value)
{
    itemTrig(value);
}

void clsMultiChannaeBox::initSweepMode()
{
    QStringList color; //每个通道的颜色
    color<<"#e0861a"<<"#99cc00"<<"#9f991a"<<"#a48033"<<"#a9664d"<<"#ae4d66"<<"#b33380"<<"#dbce8f"
        <<"#a64086"<<"#994d8d"<<"#8d5a93"<<"#806699"<<"#8073a6"<<"#8080b3"<<"#ffd400"
       <<"#145b7d"<<"#99b389"<<"#11264f"<<"#99e600"<<"#8a2e3b";

    cmbItem->clear();
    //获取所有的测试项目
    for(int i=0; i<meter->getTotleItemCount(); i++)
    {
        cmbItem->addItem(meter->getItem(i));
    }
    //清除所有的Box 在重新赋值
    for(int i=0; i< checkBoxs.length();i++)
    {
        QCheckBox *box = checkBoxs.at(i);
        box->setVisible(false);
        delete box;
    }

    checkBoxs.clear();
    // 重新初始化每一个checkBox
    for(int i=0; i< channels.split(",").length(); i++)
    {
        QString text = channels.split(",").at(i);
        if(!text.isEmpty())
        {
            QCheckBox *tmpBox = new QCheckBox(tr("通道:\t")+text);
            QColor clr(color.at(i%color.length()));

            tmpBox->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(clr.red()).arg(clr.green()).arg(clr.blue()));
            tmpBox->setFont(QFont("楷体",13));

            checkBoxs.append(tmpBox);
        }
    }
    //将每一个checkBox都放在面板上，并连接信号
    QVBoxLayout *layout = new QVBoxLayout();
    for(int i=0; i< checkBoxs.length(); i++)
    {
        layout->addWidget(checkBoxs.at(i));
        connect(checkBoxs.at(i),SIGNAL(toggled(bool)),this,SLOT(setCurveEnable(bool)));
    }
    layout->addStretch();

    QLayout *al = wdgCheckBox->layout();
    if(al!=0)
        delete al;

    wdgCheckBox->setLayout(layout);

    //获取测试的项目
    QStringList tmpItem;
    for(int i=0; i<meter->getTotleItemCount();i++)
    {
        tmpItem.append(meter->getItem(i));
    }
    //将测试的通道和项目，赋值个plotWidget
    plotWidget->setChannalAndItems(this->channels,tmpItem);
    //设置现在显示的曲线
    plotWidget->setCurrentIndex(cmbItem->currentIndex());
    //设置曲线的显示还是隐藏
    QList<bool> tmpBool;
    for(int i=0; i<checkBoxs.length();i++)
    {
        tmpBool.append(checkBoxs.at(i)->isChecked());
    }
    plotWidget->setCurveVisable(tmpBool);
}

void clsMultiChannaeBox::on_btnSelectMode_clicked()
{
    btnSignleTest->setChecked(false);
    btnSignleTest->setIcon(QIcon(":/Icons/single.png"));

    dispStactWindow->setCurrentIndex((dispStactWindow->currentIndex()+1)%2);

    if(dispStactWindow->currentIndex()==1)
    {
        initSweepMode();
    }
}

void clsMultiChannaeBox::setCurveEnable(bool /*value*/)
{
    QList<bool> tmpBool;
    for(int i=0; i<checkBoxs.length();i++)
    {
        tmpBool.append(checkBoxs.at(i)->isChecked());
    }
    plotWidget->setCurveVisable(tmpBool);
}

void clsMultiChannaeBox::on_cmbItem_currentIndexChanged(int index)
{
    plotWidget->setCurrentIndex(index);
}

void clsMultiChannaeBox::mkDataDir()
{


    QDir dir("./");
    if(dir.exists("MultiChannelDataFile"))
        return;
    else
    {
        dir.mkdir("MultiChannelDataFile");
    }
}


void clsMultiChannaeBox::on_btnOpenDataDir_clicked()
{
    QString curDir;
    curDir = QDir::currentPath();
    //qDebug()<< curDir;
    QDesktopServices::openUrl(QUrl(QString("file:///%1/MultiChannelDataFile").arg(curDir), QUrl::TolerantMode));

}

void clsMultiChannaeBox::on_btnAboutMe_clicked()
{
    frmAbout *dlg =new frmAbout(this);
    dlg->setWindowTitle(tr("关于本软件"));
    dlg->exec();
}

void clsMultiChannaeBox::on_btnSavePic_clicked()
{
    QPixmap originalPixmap = QPixmap::grabWidget(dispStactWindow);

    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("保存测试图像"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(format.toUpper())
                                                    .arg(format));
    if (!fileName.isEmpty())
        originalPixmap.save(fileName, "png");
}
