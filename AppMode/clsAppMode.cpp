#include "clsAppMode.h"
#include <QFile>
#include <QProcess>
#include "clsDownLoad.h"
#include <QDebug>
#include "clsSettings.h"
#include <QBoxLayout>
#include <QLayout>
clsAppMode::clsAppMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    readSettings();
}

void clsAppMode::on_btnCheckNewApp_clicked()
{
    clsDownLoad *dlg = new clsDownLoad(this);

    connect(dlg,SIGNAL(hasDownloadFile(QString)),this,SLOT(addApp(QString)));
    dlg->exec();
}

void clsAppMode::btnClick(QString value)
{

    QProcess *process = new QProcess(this);
    process->start(value);
}

void clsAppMode::addApp(QString appName)
{
    clsMyPushButton * btn = new clsMyPushButton;
    btn->setText(appName);

    connect(btn,SIGNAL(btnClick(QString)),this,SLOT(btnClick(QString)));

    buttons.append(btn);
    if(appWidget->layout()!=NULL)
        delete appWidget->layout();


    QVBoxLayout *layout = new QVBoxLayout();

    for(int i =0; i<buttons.length(); i++)
    {
        layout->addWidget(buttons.at(i));
    }

    layout->addStretch();

    this->appWidget->setLayout(layout);

    if(!apps.contains(appName))
        apps.append(appName);

    writeSettings();
}

void clsAppMode::readSettings()
{
    clsSettings settings;

    QString strNode ="App/";

    settings.readSetting(strNode+"DownLoadApp", apps);

    foreach (QString tmp, apps) {

        QFile file(tmp);

        if(file.exists())
            addApp(tmp);
        else
        {
            apps.removeAll(tmp);
        }
    }
}

void clsAppMode::writeSettings()
{
    clsSettings settings;

    QString strNode ="App/";

    settings.writeSetting(strNode+"DownLoadApp", apps);
}

void clsAppMode::closeEvent(QCloseEvent *)
{
    writeSettings();
}
