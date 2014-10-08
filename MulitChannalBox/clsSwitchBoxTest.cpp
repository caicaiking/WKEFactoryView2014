#include "clsSwitchBoxTest.h"
#include <QTime>
#include <QDebug>
#include "clsConnectSWBox.h"
#include "clsMultiChannaeBox.h"
clsSwitchBoxTest::clsSwitchBoxTest(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    commands = clsMultiChannaeBox::initCommand();
}

void clsSwitchBoxTest::on_btnLoop_clicked()
{
    int i=0;
    int count=0;
    QTime time=QTime::currentTime();
    while(btnLoop->isChecked())
    {
        qApp->processEvents();
        clsConnectSWBox::Instance()->sendCommand(commands.at(i));
        i=i+1;
        i=i%20;

        if(i==0)
        {
            count++;
            qDebug()<<"Use time: "<<count<<" : "<< QTime::currentTime().msecsTo(time);
        }
        qApp->processEvents();
    }
}

void clsSwitchBoxTest::on_txtChannel_valueChanged(int arg1)
{
    btnLoop->setChecked(false);
    qApp->processEvents();
    clsConnectSWBox::Instance()->sendCommand(commands.at(arg1-1));
}
