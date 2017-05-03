#include "dlgSpeed.h"
#include "NumberInput.h"

dlgSpeed::dlgSpeed(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void dlgSpeed::on_btnMax_clicked()
{
    Speed =tr("最快");
    this->accept();
}

void dlgSpeed::on_btnFast_clicked()
{
     Speed =tr("快速");
    this->accept();
}

void dlgSpeed::on_btnMed_clicked()
{
     Speed =tr("中速");
    this->accept();
}

void dlgSpeed::on_btnSlow_clicked()
{
     Speed =tr("慢速");
    this->accept();
}

void dlgSpeed::on_btnCustomer_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("设置测试速度1~255"));

    if(dlg->exec())
    {
       Speed = QString::number(int(dlg->getNumber()));

      this->btnCustomer->setText(Speed);
       this->accept();
    }
}
