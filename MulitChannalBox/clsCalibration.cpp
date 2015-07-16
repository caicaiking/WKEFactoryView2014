#include "clsCalibration.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
clsCalibration::clsCalibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    clsRS::getInst().sendCommand(":MEAS:NUM-OF-TESTS 1");
    clsRS::getInst().sendCommand(":MEAS:FUNC1 Z");
    clsRS::getInst().sendCommand(":MEAS:FUNC2 A");
    clsRS::getInst().sendCommand(":MEAS:EQU-CCT SERIES");
}



void clsCalibration::on_btnTest_clicked()
{

}

void clsCalibration::on_btnOpen_clicked()
{
    QString res = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

    QList<double> resValue = UserfulFunctions::resultPro(res);

    txtOpenZ->setText();
}

void clsCalibration::on_btnShort_clicked()
{

}
