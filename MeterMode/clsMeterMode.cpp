#include "clsMeterMode.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "cls6440MeterMode.h"

#include <QLayout>
#include "clsMeterModeFactory.h"
#include "clsRuningSettings.h"
#include "clsSetTestStep.h"
clsMeterMode::clsMeterMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    lblConnectionType->setText(clsRS::getInst().getConnectionType() + QObject::tr("连接"));
    lblTime->showTime();
    lblStatus->setStatus(IDEL);
    skWidget->setCurrentIndex(0);

    //sngTestDisplay->setMessage(tr("没有测试数据可以显示！"),0);
   // sngTestDisplay->setMessage("L,512.03nH,PASS,C,-2.003nF,FAIL,R,1.2354mΩ,PASS,C,-2.003nF,FAIL",4);
    //this->showMaximized();



}

void clsMeterMode::on_btnSetStep_clicked()
{
    clsSetTestStep *dlg = new clsSetTestStep(this);
    dlg->setWindowTitle(tr("设置测试步骤"));

    dlg->setTestSteps(this->steps);

    if(dlg->exec()==QDialog::Accepted)
    {
        steps = dlg->getTestSteps();
    }

}
