#include "cls6500TurnOffScreen.h"
#include "clsRuningSettings.h"
cls6500TurnOffScreen::cls6500TurnOffScreen()
{

}

void cls6500TurnOffScreen::doOperation()
{
    QString strCondition = this->getConditon();

    clsRS::getInst().sendCommand(QString(":METER:FAST-GPIB %1").arg(strCondition),false);
}
