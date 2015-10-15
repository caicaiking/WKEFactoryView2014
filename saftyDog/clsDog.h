#ifndef CLSDOG_H
#define CLSDOG_H

#include "DogControl.h"
#include "singleton.h"
class clsDog
{
public:
    clsDog();
    bool getName(QString &retValue);
    void setProduct(bool value=true);
    bool getVersion();
private:
    bool isProduct;

};

typedef Singleton<clsDog> SingletonDog;


#endif // CLSDOG_H
