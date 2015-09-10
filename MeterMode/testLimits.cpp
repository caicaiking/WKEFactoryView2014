#include "testLimits.h"
#include <QDebug>
testLimits::testLimits(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    label->setText(lm.showLimits("F"));

    connect(this->label,SIGNAL(Clicked()),this,SLOT(setLimit()));
}

void testLimits::on_btnEditLimits_clicked()
{
    QString st;
    qDebug()<<lm.comparaValue(12,st)<<"\tst:"<< st;

    qDebug()<<lm.toString();
}

void testLimits::setLimit()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setLimits(lm);

    if(dlg->exec()==QDialog::Accepted)
    {
        lm = dlg->getMeterLimit();
        label->setText(lm.showLimits("F"));

    }
}

