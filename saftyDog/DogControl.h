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

        "yBRjJgT5UOYzMt4InDV6OVbALoodYYSxMeI2ATgTWJafG0zdqr482PG3+"
        "VWslgAQOQuLLKJjjUyTfZGpI3sFE1qIFTAtzdnLknEfUC9Xpu1UtzNbuRT"
        "DqXDQsh8qRz4CIWNnSl5e26M1tUQlh9mcKaW8YqiXfxWe7FcUCExh5ELxTF"
        "6K1EczECwgbSTTCw4U3vY8QWlzwPf6bOG3HauTtJmirsZisoVXr7E0yP6m7"
        "xr9+ZKx2pbA4ZP+PBP88zp229vl6/Ykm9uWyU7V7vGvGTozwiBbJKzB3kY9"
        "Ca5AKMlsJX3W+LAqL4jOly+oKfLi+2IyfPm+q/ZE1jmczdNsKUhDFj/tnx4s"
        "izpgeXmEGAzBZbwxFAtj8u0H5e9Xt8W9vinGjPQyQBi8PXjw1GY9/xGNsb/7"
        "PnpRPwd0qZqyGMzJr55QyXiHRrXQ+rPQr4zWs83Ng6Eyq58LMzxu2llsNqLwy"
        "11uBj01/7Kx3yehjnjO/Eutql+v9XhB2Ycw/HUqBUEXtUiCqnmTinIFtfbVjWg"
        "jX6MjrJNOUFT6YVC2c2oAGLfnf9RKWGUUW97k0nCdWdIDUWbum2dgM5FUNI7y9"
        "22DyWqF6mYBWyROnjtBnhFtDR1Jqpop/JvjTiANl4bisDgnPNfvr8Lh2OiYhXgi"
        "JXpab1sZisGY7x9XLC6GFsN8o257Sk9sK9tabY/ZkUTf13veiwZp1eBKgfHs+KU"
        "fdL/+HCU/GkOHw6Rl/3grdQP4LlKsEFOLcH0+wD+ZOZUMVdmzB7zghCL2p4wNZV"
        "oRtucLwFsVnACitcazVVDRSm3+zaKI9au1LY1rtCn+tqIrkcpmXuJDTRRz5j8pg"
        "KDEuw9SuXdH3QEcFWLjWuV9GYgL8ztk717sgFAXcEDB4fRuDJwa1Hfyf542Lbgr"
        "8SE00ZU9UWiXrFEh8pL9WPzEsh/EUWi27h7j72AoIvJUjVFRFoCSJ+/Y5WcYU/T"
        "WqrfaXA==";

//WKE FactoryView 2014
#define ENCRYPT_BUFFER_FEATUREID1 0    //feature id which is selected
#define SOURCE_BUFFER_LENGTH1 20    //length of source string
#define ENCRYPT_BUFFER_LENGTH1 20    //length of encrypt string

const int isString1 = 1;    //This is a string buffer
/*The encrypted array is in UTF-8 format. Please convert it to proper format before using it.*/

const unsigned char encryptStrArr1[20] = {
   0xD9, 0x5A, 0x7D, 0xAB, 0xCE, 0x32, 0x40, 0x86, 0xB6, 0x6D, 0x61, 0x09, 0x21, 0x8E, 0x09, 0x75, 0x50, 0x94, 0x63, 0xD5
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
