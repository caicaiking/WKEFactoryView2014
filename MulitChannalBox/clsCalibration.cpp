#include "clsCalibration.h"

clsCalibration::clsCalibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    clsRS::getInst().sendCommand(":MEAS:NUM-OF-TESTS 2");
}

void clsCalibration::on_btnCalibration_clicked()
{

}
