#include "clsMaterialSettings.h"
#include "doubleType.h"
#include "UserfulFunctions.h"
#include "clsRuningSettings.h"
#include "NumberInput.h"
#include "MessageDialog.h"
clsMaterialSettings::clsMaterialSettings(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);

    conD=38.0;
    conTm=1.0;

    nonConTg=2.1;
    nonConTm=2.0;

    N=5;
    l=60.0;
    A=50.0;

    updateButtons();
    updateGpibCommands();
    this->myProgressBar->setVisible(false);


}

void clsMaterialSettings::on_btnClose_clicked()
{
    this->close();
}

void clsMaterialSettings::updateButtons()
{
    doubleType dt;
    dt.setData(conD);

    btnConD->setText(dt.formateToString());

    dt.setData(conTm);
    btnConTm->setText(dt.formateToString());

    dt.setData(nonConTg);
    btnNonConTg->setText(dt.formateToString());

    dt.setData(nonConTm);
    btnNonConTm->setText(dt.formateToString());

    btnN->setText(QString::number(N));

    dt.setData(l);
    btnL->setText(dt.formateToString());

    dt.setData(A);
    btnA->setText(dt.formateToString());

}

void clsMaterialSettings::on_grpContact_toggled(bool arg1)
{
    grpNoContact->setChecked(!arg1);
    updateGpibCommands();
}


void clsMaterialSettings::on_grpNoContact_toggled(bool arg1)
{
    grpContact->setChecked(!arg1);
    updateGpibCommands();
}

void clsMaterialSettings::updateGpibCommands()
{
    if(grpContact->isChecked())
    {
        clsRS::getInst().sendCommand(":MAT:TYPE E-CONT");
    }
    else
    {
        clsRS::getInst().sendCommand(":MAT:TYPE E-NONCONT");
    }
    UserfulFunctions::sleepMs(5);

    clsRS::getInst().sendCommand(QString(":MAT:D %1").arg(conD));
    UserfulFunctions::sleepMs(5);
    clsRS::getInst().sendCommand(QString(":MAT:CONT-TM %1").arg(conTm));
    UserfulFunctions::sleepMs(5);

    clsRS::getInst().sendCommand(QString(":MAT:TG %1").arg(nonConTg));
    UserfulFunctions::sleepMs(5);
    clsRS::getInst().sendCommand(QString(":MAT:NONCONT-TM %1").arg(nonConTm));
    UserfulFunctions::sleepMs(5);
    clsRS::getInst().sendCommand(QString(":MAT:N %1").arg(N));
    UserfulFunctions::sleepMs(5);
    clsRS::getInst().sendCommand(QString(":MAT:L %1").arg(l));
    UserfulFunctions::sleepMs(5);
    clsRS::getInst().sendCommand(QString(":MAT:A %1").arg(A));
}

void clsMaterialSettings::on_btnConD_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("电极直径‘D’(mm)"));
    if(dlg->exec()== QDialog::Accepted)
    {
        conD = dlg->getNumber();
        updateButtons();
        updateGpibCommands();
    }
}

void clsMaterialSettings::on_btnConTm_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("材料厚度‘Tm’(mm)"));

    if(dlg->exec()== QDialog::Accepted)
    {
        conTm = dlg->getNumber();
        updateButtons();
        updateGpibCommands();
    }
}

void clsMaterialSettings::on_btnNonConTg_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("电极间距‘Tg’(mm)"));

    if(dlg->exec()== QDialog::Accepted)
    {
        nonConTg = dlg->getNumber();
        updateButtons();
        updateGpibCommands();
        lblCg->setText(tr("没有参考数据"));
    }

}

void clsMaterialSettings::on_btnNonConTm_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("材料厚度‘Tm’(mm)"));

    if(dlg->exec()== QDialog::Accepted)
    {
        nonConTm = dlg->getNumber();
        updateButtons();
        updateGpibCommands();
    }

}

void clsMaterialSettings::on_btnN_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("绕线匝数‘N’"));

    if(dlg->exec()== QDialog::Accepted)
    {
        N = (int)dlg->getNumber();
        updateButtons();
        updateGpibCommands();
        lblLwRw->setText(tr("没有参考数据"));
    }
}

void clsMaterialSettings::on_btnL_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("平均磁路长度‘l’(mm)"));

    if(dlg->exec()== QDialog::Accepted)
    {
        l = dlg->getNumber();
        updateButtons();
        updateGpibCommands();
        lblLwRw->setText(tr("没有参考数据"));
    }
}

void clsMaterialSettings::on_btnA_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("磁环截面积‘A’(mm²)"));

    if(dlg->exec()== QDialog::Accepted)
    {
        A = dlg->getNumber();
        updateButtons();
        updateGpibCommands();
        lblLwRw->setText(tr("没有参考数据"));
    }
}

void clsMaterialSettings::on_btnMeasCg_clicked()
{
    bool isAccepted = PopUpmessage(tr("移除测试材料，保持测试电极之间为空气"),tr("测量Cg参考数据"));
    if(!isAccepted)
        return;

    clsRS::getInst().sendCommand(":MAT:MEAS-CG");
    showProgress(25);

    QString calRes = clsRS::getInst().sendCommand(":MAT:MEAS-CG?",true);
    if(calRes.toInt()==1)
    {

        lblCg->setText(tr("参考数据获取成功，\n校准时间：%1 %2")
                       .arg(QDate::currentDate().toString("yyyy-M-dd"))
                       .arg(QTime::currentTime().toString("hh:mm:ss")));

    }
    else
    {
        lblCg->setText(tr("参考数据获取失败。"));
    }
}

void clsMaterialSettings::on_btnMeasLwRw_clicked()
{
    bool isAccepted = PopUpmessage(tr("移除磁环，保持测试夹具为空气电感"),tr("测试Lw/Rw参考数据"));
    if(!isAccepted)
        return;

    clsRS::getInst().sendCommand(":MAT:MEAS-LR");
    showProgress(22);

    QString calRes = clsRS::getInst().sendCommand(":MAT:MEAS-LR?",true);
    if(calRes.toInt()==1)
    {

        lblLwRw->setText(tr("参考数据获取成功，\n校准时间：%1 %2")
                         .arg(QDate::currentDate().toString("yyyy-M-dd"))
                         .arg(QTime::currentTime().toString("hh:mm:ss")));

    }
    else
    {
        lblLwRw->setText(tr("参考数据获取失败。"));
    }
}

void clsMaterialSettings::showProgress(int i)
{
    myProgressBar->setVisible(true);

    myProgressBar->setMaximum(i);

    myProgressBar->setValue(0);

    while(myProgressBar->value()<myProgressBar->maximum())
    {
        UserfulFunctions::sleepMs(1000);
        myProgressBar->setValue(myProgressBar->value()+1);
    }

    myProgressBar->setVisible(false);
}
bool clsMaterialSettings::PopUpmessage(QString strMessage,QString strTitle)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    ms->setMessage(strMessage,strTitle);
    return (ms->exec()==QDialog::Accepted);
}
