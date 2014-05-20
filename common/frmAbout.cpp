#include "frmAbout.h"
#include <QFile>
#include <QTextStream>
frmAbout::frmAbout(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    readVersion();
}

void frmAbout::readVersion()
{
    QFile file(":/Icons/version.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString strRead = in.readAll();

    QString version =QString("<font color=red>%1</font>").arg(strRead);
    lblVersion->setText(version);

    file.close();
}


void frmAbout::on_btnClose_clicked()
{
    this->accept();
}

void frmAbout::on_btnAboutQT_clicked()
{
    qApp->aboutQt();
}