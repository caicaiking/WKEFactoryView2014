#include "clsDog.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
clsDog::clsDog()
{
}

bool clsDog::getName(QString &retValue)
{
    return getProductName(retValue);
}
