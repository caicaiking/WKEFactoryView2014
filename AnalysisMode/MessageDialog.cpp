#include "MessageDialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void MessageDialog::setMessage(QString strMessage, QString strTitle)
{
    strMessge = strMessage;
    strTitle= strTitle;
    lblMessage->setText(strMessage);
    this->setWindowTitle(strTitle);
}

void MessageDialog::on_btnOk_clicked()
{
    this->accept();
}
