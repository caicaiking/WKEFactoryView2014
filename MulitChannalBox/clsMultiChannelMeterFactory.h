#ifndef CLSMULTICHANNELMETERFACTORY
#define CLSMULTICHANNELMETERFACTORY

#include "clsMultiModeMeter.h"
#include "cls6440MultiMeterMode.h"
#include "cls6500MultiMeterMode.h"
#include "clsRuningSettings.h"
#include <QMessageBox>
#include <QApplication>
class clsMultiChannelMeterFactory{
public:
    clsMultiChannelMeterFactory(){}

    static clsMultiModeMeter * getMeter(QString meter)
    {
        if(meter.toUpper() == "6500")
        {
            //qDebug()<< "return 6500";
            return new cls6500MultiMeterMode();
        }
        else if(meter.toUpper() == "6440")
        {
            //qDebug()<< "return 6440";
            return new cls6440MultiMeterMode();
        }
        else
        {
            QMessageBox::information(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("Not support this instrument now! \n Comming soon!"));
            clsRS::getInst().disConnect();
            qApp->exit(0);
            return 0;
        }
    }
};

#endif // CLSMULTICHANNELMETERFACTORY

