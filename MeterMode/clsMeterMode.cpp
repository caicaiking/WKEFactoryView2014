#include "clsMeterMode.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
clsMeterMode::clsMeterMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    lblConnectionType->setText(clsRS::getInst().getConnectionType() + QObject::tr("连接"));
    lblTime->showTime();
    lblStatus->setStatus(IDEL);
    skWidget->setCurrentIndex(0);

    sngTestDisplay->setMessage(tr("没有测试数据可以显示！"),0);
    //sngTestDisplay->setMessage("L,512.03nH,PASS,C,-2.003nF,FAIL,R,1.2354mΩ,PASS,C,-2.003nF,FAIL",4);

}

void clsMeterMode::on_skWidget_currentChanged(int arg1)
{
    if(arg1==0)
    {
        grpSettings->setTitle(QObject::tr("测试步骤设定"));
    }
    else
    {
        grpSettings->setTitle(QObject::tr("测试结果显示"));
    }
}

void clsMeterMode::on_btnRep_clicked()
{
    skWidget->setCurrentIndex(1);

    btnRep->setIcon(QIcon(":/Icons/stop.png"));
    qApp->processEvents();
    while(btnRep->isChecked())
    {
        qDebug()<< "rep test";
        UserfulFunctions::sleepMs(150);
        qApp->processEvents();
    }
    btnRep->setIcon(QIcon(":/Icons/repeat.png"));
}

void clsMeterMode::on_btnSetup_clicked()
{
    if(!btnSetup->isChecked())
    {
        grpSetup->setVisible(true);
        btnTrig->setVisible(true);
        btnNewTask->setVisible(true);
        btnSaveTask->setVisible(true);
    }
    else
    {
        grpSetup->setVisible(false);
        btnTrig->setVisible(false);
        btnNewTask->setVisible(false);
        btnSaveTask->setVisible(false);
    }
}

void clsMeterMode::closeEvent(QCloseEvent *event)
{
    btnSetup->setChecked(false);

    event->accept();
}
