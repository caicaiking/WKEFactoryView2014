#include "clsChennalSelect.h"
#include <QGridLayout>
#include <QDebug>


clsChennalSelect::clsChennalSelect(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
 setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    chennals.clear();

    QGridLayout *layout = new QGridLayout();

    for(int i=0; i<CHENNAL_COUNT; i++)
    {
        QCheckBox *tmp = new QCheckBox();
        tmp->setFont(QFont("楷体",12));
        tmp->setText(tr("%1 通道").arg(i+1));
        tmp->setLayoutDirection(Qt::RightToLeft);
        chennals.append(tmp);

        int m=0,n=0;
        m=i%5;
        n=i/5;

        layout->addWidget(chennals.at(i),n,m);
        connect(chennals.at(i),SIGNAL(clicked()),this,SLOT(setText()));

    }

    wdgChennals->setLayout(layout);

}

void clsChennalSelect::setChennal(QString value)
{
    QStringList list = value.split(',');

    QList<int> listChennal;
    foreach (QString tmp, list) {
        listChennal.append(tmp.toInt());
    }

    for(int i=0; i<listChennal.length();i++)
    {
        if(listChennal.at(i)<CHENNAL_COUNT && listChennal.at(i)>0)
        {
            chennals.at(listChennal.at(i)-1)->setChecked(true);
        }

    }
    this->setText();

}

QString clsChennalSelect::getChennal()
{
    return txtChennal->text();
}

void clsChennalSelect::setText()
{
    QStringList listNumber;
    for(int i=0; i< chennals.length();i++ )
    {
        if(chennals.at(i)->isChecked())
            listNumber.append(QString::number(i+1));
    }

    txtChennal->setText(listNumber.join(','));
}

void clsChennalSelect::on_btnSelect10_clicked()
{
    if(btnSelect10->isChecked())
    {
        btnSelect10->setText(tr("不选前10通道"));
    }
    else
    {
        btnSelect10->setText(tr("选定前10通道"));
    }

    int maxNumber = (10<CHENNAL_COUNT?10:CHENNAL_COUNT);

    for(int i=0;i<maxNumber;i++)
    {

        chennals.at(i)->setChecked(btnSelect10->isChecked());
    }

    setText();

}

void clsChennalSelect::on_btnSelectAll_clicked()
{
    if(btnSelectAll->isChecked())
    {
        btnSelectAll->setText(tr("全不选"));
    }
    else
    {
        btnSelectAll->setText(tr("全选"));
    }

    for(int i=0; i<chennals.length(); i++)
    {
        chennals.at(i)->setChecked(btnSelectAll->isChecked());
    }
    setText();
}

void clsChennalSelect::on_btnOk_clicked()
{
    this->accept();
}

void clsChennalSelect::on_btnCancel_clicked()
{
    this->reject();
}
