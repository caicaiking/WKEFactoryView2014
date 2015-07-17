#include "clsCalibration.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
#include "clsDataProcess.h"
clsCalibration::clsCalibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    //clsRS::getInst().sendCommand(":MEAS:NUM-OF-TESTS 1");
    clsRS::getInst().sendCommand(":MEAS:FUNC:Z");
    clsRS::getInst().sendCommand(":MEAS:EQU-CCT SERIES");
    clsRS::getInst().sendCommand(":MEAS:FREQ 100000");
    clsRS::getInst().sendCommand(":MEAS:SPEED SLOW");
    clsRS::getInst().sendCommand(":FAST-GPIB ON");

}



void clsCalibration::on_btnTest_clicked()
{
    QString res = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

    QList<double> resValue = UserfulFunctions::resultPro(res);

    double z,a;
    z = resValue.at(0);
    a = resValue.at(1);
    clsDataProcess d(z,a,100000);
    d.applyOpenData(ocZ,ocA);
    d.applyShortData(scZ,scA);
    d.doCalibration();

    double c= d.getItem("C",QString("串联"));
    double dx= d.getItem("D",QString("串联"));

    doubleType dt;
    dt.setData(c);
    txtC->setText(dt.formateToString());
    dt.setData(dx);
    txtD->setText(dt.formateWithUnit(""));

    double l= d.getItem("L",QString("串联"));
    double r= d.getItem("R",QString("串联"));


    dt.setData(l);
    txtL->setText(dt.formateToString());
    dt.setData(r);
    txtR->setText(dt.formateToString());
}

void clsCalibration::on_btnOpen_clicked()
{
    QString res = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

    QList<double> resValue = UserfulFunctions::resultPro(res);

    doubleType dt;
    dt.setData(resValue.at(0));

    txtOpenZ->setText(dt.formateToString());
    ocZ = dt.Data();
    dt.setData(resValue.at(1));
    txtOpenA->setText(dt.formateToString());
    ocA = dt.Data();
}

void clsCalibration::on_btnShort_clicked()
{
    QString res = clsRS::getInst().sendCommand(":MEAS:TRIG",true);

    QList<double> resValue = UserfulFunctions::resultPro(res);

    doubleType dt;
    dt.setData(resValue.at(0));

    txtShortZ->setText(dt.formateToString());
    scZ = dt.Data();
    dt.setData(resValue.at(1));
    txtShortA->setText(dt.formateToString());
    scA = dt.Data();
}
