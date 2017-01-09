#include "clsSwitchBoxTest.h"
#include <QTime>
#include <QDebug>
#include "clsConnectSWBox.h"
#include "clsMultiChannaeBox.h"
#include <QCloseEvent>
clsSwitchBoxTest::clsSwitchBoxTest(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    terminal =2;
}

void clsSwitchBoxTest::on_btnLoop_clicked()
{
    int i=0;

    //注释的语句是用来测试切换的时间的。

    while(btnLoop->isChecked())
    {
        qApp->processEvents();
        i=i+1;
        QTime time=QTime::currentTime();
        clsConnectSWBox::Instance()->selectChannel(i);
        qDebug()<< time.msecsTo(QTime::currentTime());
        i=i%(20*2/terminal);

        qApp->processEvents();
    }
}

void clsSwitchBoxTest::on_txtChannel_valueChanged(int arg1)
{
    btnLoop->setChecked(false);
    qApp->processEvents();
    clsConnectSWBox::Instance()->selectChannel(arg1);
}

/*!
 * \brief clsSwitchBoxTest::closeEvent
 * 当关闭本测试窗口的时候，中断正在重复运行的测试窗口。
 * \param e
 */
void clsSwitchBoxTest::closeEvent(QCloseEvent *e)
{
    this->btnLoop->setChecked(false);
    qApp->processEvents();
    e->accept();
}

void clsSwitchBoxTest::on_btnChangeTerminal_clicked()
{
    int ret =clsConnectSWBox::Instance()->setTerminal(2);
    setTerminal(2);
    qDebug()<<ret;
}

void clsSwitchBoxTest::on_btnChangeTerminal4_clicked()
{
    int ret =clsConnectSWBox::Instance()->setTerminal(4);
    setTerminal(4);
    qDebug()<<ret;
}

void clsSwitchBoxTest::on_btnSetPass_clicked()
{
    int ret =clsConnectSWBox::Instance()->setChannelStatus(txtChannel->value(),
                                                           PASS);
    qDebug()<<ret;
}

void clsSwitchBoxTest::on_btnSetFail_clicked()
{
    int ret =clsConnectSWBox::Instance()->setChannelStatus(txtChannel->value(),
                                                           FAIL);
    qDebug()<<ret;
}

void clsSwitchBoxTest::on_btnSendCommand_clicked()
{
    int ret =clsConnectSWBox::Instance()->sendCommand(txtCommand->text());
    qDebug()<<ret;
}

void clsSwitchBoxTest::setTerminal(int value)
{
    this->terminal=value;

    if(terminal==2)
        txtChannel->setMaximum(20);
    else
        txtChannel->setMaximum(10);
}

void clsSwitchBoxTest::on_btnSetIdel_clicked()
{
    int ret =clsConnectSWBox::Instance()->setChannelStatus(txtChannel->value(),
                                                           IDEL);
    qDebug()<<ret;
}
