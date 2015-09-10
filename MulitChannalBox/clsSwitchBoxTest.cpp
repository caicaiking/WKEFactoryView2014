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
    commands = clsMultiChannaeBox::initCommand();
}

void clsSwitchBoxTest::on_btnLoop_clicked()
{
    int i=0;
    int count=0;
    //注释的语句是用来测试切换的时间的。
    // QTime time=QTime::currentTime();
    while(btnLoop->isChecked())
    {
        qApp->processEvents();
        clsConnectSWBox::Instance()->sendCommand(i);
        i=i+1;
        i=i%20;

        if(i==0)
        {
            count++;
            // qDebug()<<"Use time: "<<count<<" : "<< QTime::currentTime().msecsTo(time);
        }
        qApp->processEvents();
    }
}

void clsSwitchBoxTest::on_txtChannel_valueChanged(int arg1)
{
    btnLoop->setChecked(false);
    qApp->processEvents();
    clsConnectSWBox::Instance()->sendCommand(arg1-1);
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
