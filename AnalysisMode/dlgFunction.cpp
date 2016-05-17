#include "dlgFunction.h"

dlgFunction::dlgFunction(bool btnOffVisible, bool btnRdcVisible, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->chkMaterial->setVisible(false);
    grpMaterial->setVisible(false);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->setMinimumSize(this->sizeHint());
    //    this->btnOFF->setVisible(btnOffVisible);
    //    this->btnRdc->setVisible(btnRdcVisible);

    connect(this->chkMaterial,SIGNAL(clicked(bool)),this->grpMaterial,SLOT(setVisible(bool)));

}

void dlgFunction::setMateralFunction(bool value)
{
    chkMaterial->setVisible(value);
}

void dlgFunction::on_btnC_clicked()
{
    strItem="C";
    this->accept();
}

void dlgFunction::on_btnD_clicked()
{
    strItem="D";
    this->accept();
}

void dlgFunction::on_btnL_clicked()
{
    strItem="L";
    this->accept();
}

void dlgFunction::on_btnQ_clicked()
{
    strItem="Q";
    this->accept();
}

void dlgFunction::on_btnR_clicked()
{
    strItem="R";
    this->accept();
}

void dlgFunction::on_btnX_clicked()
{
    strItem="X";
    this->accept();
}

void dlgFunction::on_btnG_clicked()
{
    strItem="G";
    this->accept();
}

void dlgFunction::on_btnB_clicked()
{
    strItem="B";
    this->accept();
}

void dlgFunction::on_btnZ_clicked()
{
    strItem="Z";
    this->accept();
}

void dlgFunction::on_btnA_clicked()
{
    strItem=QString("θ");
    this->accept();
}

void dlgFunction::on_btnY_clicked()
{
    strItem="Y";
    this->accept();
}

//void dlgFunction::on_btnRdc_clicked()
//{
//    strItem="RDC";
//    this->accept();
//}

void dlgFunction::on_btnCancel_clicked()
{
    this->reject();
}

//void dlgFunction::on_btnOFF_clicked()
//{
//    strItem="OFF";
//    this->accept();
//}



void dlgFunction::on_btnE1R_clicked()
{
    strItem= btnE1R->text();
    this->accept();
}

void dlgFunction::on_btnE2R_clicked()
{
    strItem= btnE2R->text();
    this->accept();
}

void dlgFunction::on_btnDe_clicked()
{
    strItem= btnDe->text();
    this->accept();
}

void dlgFunction::on_btnU1R_clicked()
{
    strItem= btnU1R->text();
    this->accept();
}

void dlgFunction::on_btnU2R_clicked()
{
    strItem= btnU2R->text();
    this->accept();
}

void dlgFunction::on_btnDu_clicked()
{
    strItem= btnDu->text();
    this->accept();
}
