#include "dlgSetupOp.h"
#include <QFileDialog>
dlgSetupOp::dlgSetupOp(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

void dlgSetupOp::on_btnSaveSettingsFile_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(
                this, tr("保存配置文件"), fileName,
               tr( "配置文件(*.ini)"), NULL/*, QFileDialog::DontConfirmOverwrite*/);

    if(fileName.isEmpty())
        this->reject();

    intSelect =1;
    this->strSelectFilePath = fileName;
    this->accept();
}

void dlgSetupOp::on_btnOpenSettingsFile_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(
                this, tr("打开配置文件"), fileName,
                tr("配置文件(*.ini)"), NULL/*, QFileDialog::DontConfirmOverwrite*/);

    if(fileName.isEmpty())
        this->reject();

    this->strSelectFilePath = fileName;

    intSelect=0;

    this->accept();
}

void dlgSetupOp::on_btnClose_clicked()
{
   this->reject();
}
