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
    //在做产品的时候要却掉这个；

    if(isProduct == false)
    {
        retValue = "WKE FactoryView 2014";
        return true;
    }
    else
    {
        retValue = "WKE FactoryView 2014";
        return true;
    }
       // return getProductName(retValue);
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
