#include "clsShowReport.h"
#include <QDebug>

clsShowReport::clsShowReport(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void clsShowReport::setData(clsDataStore *value)
{
    this->result= value;

    txtStep->setMaximum(result->getStepCount()-1);
    txtItem->setMaximum(result->getItemCount(txtStep->value())-1);

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
