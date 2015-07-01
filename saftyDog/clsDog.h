#ifndef CLSDOG_H
#define CLSDOG_H

#include "DogControl.h"
static bool isProduct= true;
class clsDog
{
public:
    clsDog();
    static bool getName(QString &retValue);
    static void setProduct(bool value=true);

};



#endif // CLSDOG_H
