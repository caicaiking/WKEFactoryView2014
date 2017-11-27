#include "cls6440TurnOffScreen.h"
#include "clsRuningSettings.h"
cls6440TurnOffScreen::cls6440TurnOffScreen()
{

}

void cls6440TurnOffScreen::doOperation()
{
    QString strCondition = this->getConditon();

    clsRS::getInst().sendCommand(QString(":FAST-GPIB %1").arg(strCondition),false);


}
