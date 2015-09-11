#include "clsTestResult.h"

clsTestResult::clsTestResult()
{

}

int clsTestResult::getStepCount()
{
    return this->steps.count();
}

int clsTestResult::getItemCount(int intStep)
{
    if(intStep < getStepCount())
    {
        meter->setCondition(steps.at(intStep));
        return meter->getCountTestItems();
    }
    else
    {
        return 0;
    }
}

QList<double> clsTestResult::getData(int intStep, int intItem)
{
    QList<double> value;

    if(intStep< getStepCount() && intItem < getItemCount(intStep))
    {
        for(int i=0; i< result.length();i++)
        {
            value.append(result.at(i).data.at(intStep).at(intItem));
        }
        return value;
    }

    return value;

}

clsMeterLimit clsTestResult::getLimit(int intStep, int intItem)
{
    if(intStep < getStepCount() && intItem < getItemCount(intStep))
    {
        meter->setCondition(steps.at(intStep));
        return meter->getLimit(intItem);
    }
    else
    {
        return clsMeterLimit();
    }

}

QString clsTestResult::getItem(int intStep, int intItem)
{
    if(intStep < getStepCount() && intItem < getItemCount(intStep))
    {
        meter->setCondition(steps.at(intStep));
        return meter->getItem(intItem);
    }
    else
    {
        return "";
    }
}

void clsTestResult::setMeter(WKEMeterMode *value)
{
    this->meter = value;
}

void clsTestResult::setSteps(const QStringList value)
{
    if(this->steps != value)
        result.clear();

    this->steps = value;
}

void clsTestResult::addTestData(const TESTDATA_STRUCT data)
{
    result.append(data);
}


QString clsTestResult::getConditon(int i)
{
    return this->steps.at(i);
}

WKEMeterMode *clsTestResult::getMeter()
{
    return this->meter;
}

int clsTestResult::getTestCount()
{
    return result.length();
}

TESTDATA_STRUCT clsTestResult::getTestData(int i)
{
    return result.at(i);
}

