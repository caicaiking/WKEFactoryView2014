#include "clsMultiChannaeBox.h"
#include <math.h>
#include <QTime>
#include "clsSwitchBoxTest.h"
#include "cls4300MeterMode.h"
clsMultiChannaeBox::clsMultiChannaeBox(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    commands=initCommand();

    bool initCom= clsConnectSWBox::Instance()->initSerialPort();
    btnOpen->setEnabled(!initCom);
    btnSwitchBoxTest->setEnabled(initCom);

    meter = new cls4300MeterMode();
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
