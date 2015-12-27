#include "clsDog.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <QDebug>
clsDog::clsDog()
{
    isProduct= true;
}

bool clsDog::getName(QString &retValue)
{
    if(isProduct == false)
    {
        retValue = "WKE FactoryView 2014";
        return true;
    }
    else
    {
        return getProductName(retValue);
        //return true;
    }
}

void clsDog::setProduct(bool value)
{
   isProduct= value;
   //qDebug()<<isProduct;
}

bool clsDog::getVersion()
{
    return this->isProduct;
}
