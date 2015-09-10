#include "clsMeterUnit.h"
#include "UserfulFunctions.h"
clsMeterUnit::clsMeterUnit(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
 setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    btnNA->setVisible(false);
}

void clsMeterUnit::setOFFEnable(bool value)
{
    btnNA->setVisible(value);
}

void clsMeterUnit::on_btnm_clicked()
{
    strSuffix="m";
    this->accept();
}

void clsMeterUnit::on_btnG_clicked()
{
    strSuffix="G";
    this->accept();
}

void clsMeterUnit::on_btnM_clicked()
{
    strSuffix="M";
    this->accept();
}

void clsMeterUnit::on_btnk_clicked()
{
    strSuffix="k";
    this->accept();
}

void clsMeterUnit::on_btn1_clicked()
{
    strSuffix="";
    this->accept();
}

void clsMeterUnit::on_btnu_clicked()
{
    strSuffix="u";
    this->accept();
}

void clsMeterUnit::on_btnn_clicked()
{
    strSuffix="n";
    this->accept();
}

void clsMeterUnit::on_btnp_clicked()
{
    strSuffix="p";
    this->accept();
}

void clsMeterUnit::setItem(QString item)
{
    QString tmpUinit = UserfulFunctions::getSuffix(item);

    foreach (QObject *tmp, this->children()) {
        QPushButton *btn = qobject_cast<QPushButton *>(tmp);

        if(btn)
            btn->setText(btn->text()+tmpUinit);
    }

    this->btnNA->setText(tr("OFF"));
}



QString clsMeterUnit::getSuffix()
{
    return this->strSuffix;
}

void clsMeterUnit::on_btnNA_clicked()
{
    strSuffix ="OFF";
    this->accept();
}
