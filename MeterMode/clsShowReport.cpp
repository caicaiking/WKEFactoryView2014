#include "clsShowReport.h"
#include <QDebug>
#include <QMessageBox>
#include "doubleType.h"
#include "clsRuningSettings.h"
clsShowReport::clsShowReport(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void clsShowReport::setData(clsDataStore *value)
{
    this->result= value;

    if(this->result->getStepCount() == 0)
    {
        QMessageBox::warning(this,tr("导出报表"),tr("没有可用的数据，用于导出报表！"));
        this->close();
    }
    else
    {
        txtStep->setMaximum(result->getStepCount()-1);
        txtItem->setMaximum(result->getItemCount(txtStep->value())-1);
        this->updateButtons();
        this->exec();
    }

}

void clsShowReport::on_btnShow_clicked()
{
    QString value;
    QList<double> stepRes1 =result->getData(txtStep->value(),txtItem->value());
    for(int i=0; i< stepRes1.length();i++)
    {
        value.append(QString("%1,").arg(stepRes1.at(i)));
    }

    value.append(QString("总计：%1").arg(stepRes1.length()));



    textEdit->setPlainText(value);
}

void clsShowReport::on_txtStep_valueChanged(int arg1)
{
    txtItem->setMaximum(result->getItemCount(arg1)-1);
}


void clsShowReport::updateButtons()
{
    txtNumber->setText(QString::number(result->getTestCount()));
    txtIntrument->setText(clsRS::getInst().instrumentModel);


    int stepCount = result->getStepCount();
    if(stepCount==0)
        return;
    QString conditon = result->getConditon(0);

    result->getMeter()->setCondition(conditon);
    txtTestFreq->setText(result->getMeter()->getFreq());




}

void clsShowReport::on_btnClose_clicked()
{
    this->reject();
}
