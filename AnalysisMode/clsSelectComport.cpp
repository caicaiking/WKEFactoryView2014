#include "clsSelectComport.h"

clsSelectComport::clsSelectComport(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void clsSelectComport::setComports(QStringList ports)
{
    this->cmbComport->addItems(ports);

}

void clsSelectComport::on_btnOk_clicked()
{
    portSelected = cmbComport->currentText();
    this->accept();
}

QString clsSelectComport::getPortSelected() const
{
    return portSelected;
}

void clsSelectComport::on_btnCancel_clicked()
{
    this->reject();
}
