#include "frmParameter.h"
#include "clsSettings.h"

frmParameter::frmParameter(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    readSettings();
}

void frmParameter::readSettings()
{
    QString strNode;
    clsSettings settings;
    bool blTemp;
    strNode ="Parameters/";

    settings.readSetting(strNode+"R",blTemp);
    R->setChecked(blTemp);
    settings.readSetting(strNode+"X",blTemp);
    X->setChecked(blTemp);
    settings.readSetting(strNode+"C",blTemp);
    C->setChecked(blTemp);
    settings.readSetting(strNode+"D",blTemp);
    D->setChecked(blTemp);
    settings.readSetting(strNode+"L",blTemp);
    L->setChecked(blTemp);
    settings.readSetting(strNode+"Q",blTemp);
    Q->setChecked(blTemp);
    settings.readSetting(strNode+"B",blTemp);
    B->setChecked(blTemp);
    settings.readSetting(strNode+"G",blTemp);
    G->setChecked(blTemp);
    settings.readSetting(strNode+"Y",blTemp);
    Y->setChecked(blTemp);

}
void frmParameter::writeSettings()
{
    QString strNode;

    clsSettings settings;
    strNode ="Parameters/";

    settings.writeSetting(strNode+"R",R->isChecked());
    settings.writeSetting(strNode+"X",X->isChecked());
    settings.writeSetting(strNode+"C",C->isChecked());
    settings.writeSetting(strNode+"D",D->isChecked());
    settings.writeSetting(strNode+"L",L->isChecked());
    settings.writeSetting(strNode+"Q",Q->isChecked());
    settings.writeSetting(strNode+"B",B->isChecked());
    settings.writeSetting(strNode+"G",G->isChecked());
    settings.writeSetting(strNode+"Y",Y->isChecked());
}

void frmParameter::on_btnOk_clicked()
{
    if(R->isChecked())
        values.append("R");

    if(X->isChecked())
        values.append("X");

    if(C->isChecked())
        values.append("C");

    if(D->isChecked())
        values.append("D");

    if(L->isChecked())
        values.append("L");

    if(Q->isChecked())
        values.append("Q");

    if(B->isChecked())
        values.append("B");

    if(G->isChecked())
        values.append("G");

    if(Y->isChecked())
        values.append("Y");

    writeSettings();
    this->accept();
}
QStringList frmParameter::getValues() const
{
    return values;
}

void frmParameter::setValues(const QStringList &value)
{
    values = value;
}

