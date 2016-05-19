#ifndef DOGCONTROL_H
#define DOGCONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#include "dog_api.h"
#include <QString>

#define DUMP_COLS_PER_ROW     16
#define DUMP_SPACE_COLS        8

const unsigned char vendor_code[] =
       "t+8SQJkRWRO9dKx+gnFAL+iS/s7oK9heJc3zlkfN3m83mPfKF4GWTFmX+4fdMr5/XVUE4NRYeP+F4T"
       "aquLZtXmpdb+NwjkmJte0A1WoU9czpeS/96wdSHGpYln3ldXSLQccrZaNpipJUI/tMlCOlN6Vl5o3u"
       "Jv01TwuPo2T2fMnLzjYFB6nzRgLEL3yEcWmwPld4ap3x34TF09xyeyfGeoVbyNtoc6PimyptgAo11z"
       "XdQdE7TxMJeGnffMBAT7Oh4imAsyf2/WgmQZ/tiSUFF4eIiwEfdicAhwIS8I1hK9FEnDmk1jdsVkbc"
       "M+YL/mSN2VW4YeXTFg/BduOYg9xY+HlyjDp1laWtWzBo1mqkPuT4WZmzMgweI9iTH3T8i2K+PBB6Hi"
       "Cc8rAyIJv1h5cgF2wqaKeKAMVSWgGeEVxv+EcOt/por5mFw0hiArx/6Ycm6Rccy2/NbRY2FQGpiPrC"
       "E59OZMs9vdEkUJ36pCF1Crg5oW5HazJsWQ/HpAxOSpzj8uGPiHcmBtFqOUn43CzFB/gWhHpMhOLnsK"
       "+GYGLQ+9b6aN0HQRLOVKQqHy70wN51+XgwGka4EW7fWLG8xDAEBTuVqWqaA+mNJaF5rGrmykT1FSjc"
       "/kq5Mo8GvHA0Nqz9dS4ulumYVpz866U0hsph4qd1KyT2Mqjml0HRNFXcCduw3nQAo+e20Y4Ukh03//"
       "5pOes1qVqxwb/6SxcB0EmQmC5FRVGn6Uk/UwSEbhsZM5Rh/pzryHfOpzQt0Iec0ykEzoGpMCpJfqUj"
       "OgDxj51aP5BQDkemzCPl1Ct9zQz+MFNvlCftfOUWDGO3W7w16n0Y5ogrnH8a0sT6ZlR+Pojo3kP/p2"
       "GQxaPQkxJxcUaZg35OlRoWPpqxwkT/SCNOSCuOOEQEW8rju3GjG3RD+tCL/fiRPz74QKwC6yFnlc7C"
       "ZxcGODFKqlRKfDquJy9ueBAc/h3ZBZJ9iaN7hs9rRw6xIA==";

//WKE FactoryView 2014
#define ENCRYPT_BUFFER_FEATUREID1 1    //feature id which is selected
#define SOURCE_BUFFER_LENGTH1 20    //length of source string
#define ENCRYPT_BUFFER_LENGTH1 20    //length of encrypt string

const int isString1 = 1;    //This is a string buffer
/*The encrypted array is in UTF-8 format. Please convert it to proper format before using it.*/

const unsigned char encryptStrArr1[20] = {
   0x54, 0x76, 0x04, 0xA9, 0x57, 0x08, 0xA9, 0x71, 0x71, 0xDE, 0x77, 0xC5, 0x18, 0xC5, 0x6D, 0x45, 0x26, 0x0B, 0x46, 0xF3
 };

typedef __int32 ENCRYPT_DATA_TYPE1;
#define SAFE_FREE(p) { if(p) { free(p); (p) = NULL; } }
dog_status_t CheckKey();
dog_status_t DecryptConst();
dog_status_t DecryptString(QString &retString);
ENCRYPT_DATA_TYPE1 getConstValue(unsigned char *bufdata);
void getStringValue(unsigned char *bufData);
void dump(unsigned char *data, unsigned int datalen, char *margin);
void dump_ascii(unsigned char *data, unsigned int datalen, char *margin);
void dump_hex(unsigned char *data, unsigned int datalen, char *margin);
bool getProductName(QString & strRet);

#endif // DOGCONTROL_H
