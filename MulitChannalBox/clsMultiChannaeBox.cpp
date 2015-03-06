#include "clsMultiChannaeBox.h"
#include <math.h>
#include <QTime>
#include "clsSwitchBoxTest.h"
#include "cls4300MeterMode.h"
#include <QFile>
#include <QDir>
#include <Quazip/JlCompress.h>
#include <QFileDialog>
#include "clsChennalSelect.h"
#include <QTime>
#include "UserfulFunctions.h"
clsMultiChannaeBox::clsMultiChannaeBox(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    commands=initCommand();

    bool initCom= clsConnectSWBox::Instance()->initSerialPort();
    btnOpen->setEnabled(!initCom);
    btnSwitchBoxTest->setEnabled(initCom);

    meter = new cls4300MeterMode();

    chennal="1";
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
    this->showMaximized();
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
    meter->setWindowModality(Qt::ApplicationModal);
    meter->show();


}

/*!
 * \brief clsMultiChannaeBox::on_btnSave_clicked
 * 保存设定
 */
void clsMultiChannaeBox::on_btnSave_clicked()
{


    if(strSaveFileName.isEmpty())
    {
        strSaveFileName = "untitled.wkm";
    }

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

    QFile file("./tmp/Test1.xml");

    if(file.open(QFile::WriteOnly))
    {
        file.write(meter->getTestCondition().toStdString().c_str());
        file.close();
    }

    QDir dirFile("./tmp");
    QStringList list=dirFile.entryList(QStringList()<<"*.xml",QDir::Files);

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
    QStringList list=dirFile.entryList(QStringList()<<"*.xml",QDir::Files);

    if(!list.contains("Test1.xml"))
        return;
    meter->setCondition("./tmp/Test1.xml");

    QStringList files;
    foreach (QString item, list) {
        QString a ="./tmp/"+ item;
        files.append(a);
    }

    //删除所有生成的文件
    foreach (QString item , files) {
        dir.remove(item);
    }

    //  qDebug()<<fileName;
}

void clsMultiChannaeBox::on_btnSignleTest_clicked()
{
    // QTime tmp = QTime::currentTime();
    setIdeal();
    for(int i=0; i< pannel.length();i++)
    {
        clsConnectSWBox::Instance()->sendCommand(pannel.at(i)->number()-1);
        UserfulFunctions::sleepMs(5);
        QString strResult= meter->trig();
        pannel.at(i)->setTestResult(strResult);

        //  qDebug()<<strResult;
    }

    //qDebug()<<"Time: "<< tmp.msecsTo(QTime::currentTime());

}

void clsMultiChannaeBox::setIdeal()
{
    for(int i=0; i< pannel.length();i++)
        pannel.at(i)->clearAll();
}


void clsMultiChannaeBox::on_btnSelectChennal_clicked()
{
    clsChennalSelect dlg(this);
    dlg.setChennal(this->chennal);
    if(dlg.exec())
    {
        this->chennal=dlg.getChennal();
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

    foreach (QString tmp, chennal.split(",")) {
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
