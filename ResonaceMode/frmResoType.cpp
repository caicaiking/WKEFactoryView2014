#include "frmResoType.h"
#include "clsRuningSettings.h"
frmResoType::frmResoType(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    //如果要使用此功能，需要测试，好像有一些Bug。
    this->btnCrystal->setEnabled(getResOption());
}

QString frmResoType::getResonanceType()
{
    return this->res;
}

void frmResoType::on_btnSeries_clicked()
{
    this->res= btnSeries->text();
    this->accept();
}

void frmResoType::on_btnParallel_clicked()
{
    this->res=btnParallel->text();
    this->accept();
}

void frmResoType::on_btnCrystal_clicked()
{
    this->res=this->btnCrystal->text();
    this->accept();
}

bool frmResoType::getResOption()
{
    if(clsRS::getInst().meterSeries !="6500")
        return false;

    QString strOption= clsRS::getInst().sendCommand("*OPT2?",true);

    if(strOption.length()>6)
    {
        return  (strOption.at(5)=='1');
    }
    else
        return false;
}
