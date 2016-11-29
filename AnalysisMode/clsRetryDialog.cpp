#include "clsRetryDialog.h"

clsRetryDialog::clsRetryDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

}

void clsRetryDialog::setMessage(QString value)
{
    this->lblMsg->setText(value);
}

void clsRetryDialog::on_btnGiveUp_clicked()
{
    this->reject();

}

void clsRetryDialog::on_btnRetry_clicked()
{
    this->accept();
}
