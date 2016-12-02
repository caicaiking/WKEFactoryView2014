#include "clsSingleTrig.h"
#include "clsDataProcess.h"
#include "clsCalibrationDbOp.h"

clsSingleTrig::clsSingleTrig()
{
}
int clsSingleTrig::getChannel() const
{
    return channel;
}

void clsSingleTrig::setChannel(int value)
{
    channel = value;
}
double clsSingleTrig::getFrequency() const
{
    return frequency;
}

void clsSingleTrig::setFrequency(double value)
{
    frequency = value;
}
double clsSingleTrig::getZm() const
{
    return Zm;
}

void clsSingleTrig::setZm(double value)
{
    Zm = value;
}
double clsSingleTrig::getAm() const
{
    return Am;
}

void clsSingleTrig::setAm(double value)
{
    Am = value;
}
bool clsSingleTrig::getBlUseLoad() const
{
    return blUseLoad;
}

void clsSingleTrig::setBlUseLoad(bool value)
{
    blUseLoad = value;
}

void clsSingleTrig::doLoadCalibration()
{
    Zm = Zm +(Zm==0? 1.0E-9:0.0);
    Am = Am +(Am==0? 1.0E-9:0.0);

    QList<double> openData = clsCalDb::getInst()->getCalData(frequency,channel,"O");
    QList<double> shortData = clsCalDb::getInst()->getCalData(frequency,channel,"S");
    QList<double> loadData = clsCalDb::getInst()->getCalData(frequency,channel,"Lm");
    QList<double> stdData = clsCalDb::getInst()->getCalData(frequency,channel,"Ls");

    clsDataProcess d(Zm,Am,frequency);
    if((openData.length() ==2) && (shortData.length() ==2))
    {
        d.applyOpenData(openData.at(0),openData.at(1));
        d.applyShortData(shortData.at(0),shortData.at(1));

        if((loadData.length()==2) && (stdData.length()==2) && blUseLoad)
        {
            d.applyLoadData(loadData.at(0),loadData.at(1));
            d.applyStdData(stdData.at(0),stdData.at(1));
            d.useLoadData(true);
            d.doCalibration();
        }
        else
        {
            d.useLoadData(false);
            d.doCalibration();
        }
    }

    Zm = d.getItem("Z",QObject::tr("串联"));
    Am = d.getItem("A",QObject::tr("串联"));
}

void clsSingleTrig::doRCCalibration()
{
    Zm = Zm +(Zm==0? 1.0E-9:0.0);
    Am = Am +(Am==0? 1.0E-9:0.0);



    QList<double> openData = clsCalDb::getInst()->getCalData(frequency,channel,"O");
    QList<double> shortData = clsCalDb::getInst()->getCalData(frequency,channel,"S");
    QList<double> z100Rref = clsCalDb::getInst()->getCalData(10000.0,channel,"HF_100RRef");
    QList<double> z100CR = clsCalDb::getInst()->getCalData(frequency,channel,"HF_C100R");
    QList<double> z100Pref = clsCalDb::getInst()->getCalData(10000.0,channel,"HF_100PRef");
    QList<double> z100CP = clsCalDb::getInst()->getCalData(frequency,channel,"HF_C100P");

    if((openData.length() !=2) ||(shortData.length() !=2))
        return;
    clsDataProcess zop(Zm,Am,frequency);

    zop.applyOpenData(openData.at(0),openData.at(1));
    zop.applyShortData(shortData.at(0),shortData.at(1));
    zop.useLoadData(false);
    zop.doCalibration();

    Zm=zop.getItem("Z",QObject::tr("串联")) ;
    Am=zop.getItem("A",QObject::tr("串联"));


    if((z100Rref.length() !=2) || (z100CR.length() !=2) || (z100Pref.length() !=2) || (z100CP.length() !=2 ))
        return;

    clsDataProcess z100CRTrimed(z100CR.first(),z100CR.last(),frequency);

    z100CRTrimed.applyOpenData(openData.at(0),openData.at(1));
    z100CRTrimed.applyShortData(shortData.at(0),shortData.at(1));
    z100CRTrimed.useLoadData(false);
    z100CRTrimed.doCalibration();

    clsDataProcess z100CPTrimed(z100CP.first(),z100CP.last(),frequency);

    z100CPTrimed.applyOpenData(openData.at(0),openData.at(1));
    z100CPTrimed.applyShortData(shortData.at(0),shortData.at(1));
    z100CPTrimed.useLoadData(false);
    z100CPTrimed.doCalibration();



    double zFactor = z100CRTrimed.getItem("Z",QObject::tr("串联")) / z100Rref.first();
    Zm = Zm/zFactor;

    double aFactor = z100CPTrimed.getItem("A",QObject::tr("串联"))  - z100Pref.last();
    Am = Am - aFactor-(z100Pref.last()+90.000);

    clsDataProcess end(Zm,Am,frequency);
    end.applyOpenData(openData.at(0),openData.at(1));
    end.applyShortData(shortData.at(0),shortData.at(1));
    end.useLoadData(false);
   // end.doCalibration();

    Zm=end.getItem("Z",QObject::tr("串联")) ;
    Am=end.getItem("A",QObject::tr("串联"));


}

double clsSingleTrig::getItem(QString item, QString equcct)
{
     clsDataProcess d(Zm,Am,frequency);

    return d.getItem(item,equcct);
}





