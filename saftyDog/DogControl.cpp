#include "DogControl.h"
#include <QDebug>


/****************************************************************************
*
* Demo program for SuperDog license detect
*
* Copyright (C) 2012 SafeNet, Inc. All rights reserved.
*
****************************************************************************/


bool getProductName (QString & strRet)
{
    //decrypt string or raw data using SuperDog
  if( DecryptString(strRet)==0)
      return true;
  else
      return false;
}


dog_status_t DecryptString(QString &retString)
{
    dog_status_t   status;
    dog_handle_t   hDog;
    //int nStatus   = 0;
    //int i = 0;
    unsigned char *bufData = NULL;

    /************************************************************************
    * dog_login
    *   establishes a context for SuperDog
    */
    status = dog_login(ENCRYPT_BUFFER_FEATUREID1,
        vendor_code,
        &hDog);
    if (DOG_STATUS_OK != status)
    {
//        switch (status)
//        {
//        case DOG_INV_VCODE:
//           // printf("Invalid vendor code.\n");
//            break;

//        case DOG_UNKNOWN_VCODE:
//           //  printf("Vendor Code not recognized by API.\n");
//            break;

//        default:
//           //  printf("Login to feature: %d failed with status %d\n", ENCRYPT_BUFFER_FEATUREID1, status );
//        }
        return status;
    }

    bufData = (unsigned char *)malloc(ENCRYPT_BUFFER_LENGTH1+1);
    memset(bufData, 0, ENCRYPT_BUFFER_LENGTH1+1);
    memcpy(bufData, encryptStrArr1, ENCRYPT_BUFFER_LENGTH1);

    /*
    * dog_decrypt
    *   decrypts a block of data which is encrypted
    *   (minimum buffer size is 16 bytes)
    */
    status = dog_decrypt(hDog, bufData, ENCRYPT_BUFFER_LENGTH1);
    if (DOG_STATUS_OK != status)
    {
        SAFE_FREE(bufData);
        dog_logout(hDog);
        return status;
    }

    //If source string length is less than 16, we need add 0 to the remained buffer
    if(ENCRYPT_BUFFER_LENGTH1 > SOURCE_BUFFER_LENGTH1)
    {
        memset(bufData+SOURCE_BUFFER_LENGTH1, 0, ENCRYPT_BUFFER_LENGTH1-SOURCE_BUFFER_LENGTH1);
    }

    //Use the decrypted data do some operation
    if(0 == isString1)
    {
       //  printf("The decrypted buffer data is below :\n");
        dump(bufData, SOURCE_BUFFER_LENGTH1, "    ");
    }
    else
    {
        getStringValue(bufData);
//        printf("The decrypted string is: \"%s\".\n", bufData);
    }

    retString=(const char*)bufData;

    //qDebug()<<retString;

    SAFE_FREE(bufData);
    dog_logout(hDog);

    return status;
}

ENCRYPT_DATA_TYPE1 getConstValue(unsigned char *bufdata)
{
    return *(ENCRYPT_DATA_TYPE1 *)bufdata;
}

void getStringValue(unsigned char *bufData)
{
    int wcsLen = 0;
    wchar_t* wszString = NULL;
    int textlen = 0;

    //Change UTF8 format string to unicode
    wcsLen = MultiByteToWideChar(CP_UTF8, 0,( CHAR *) bufData, ENCRYPT_BUFFER_LENGTH1, NULL, 0);
    wszString = (wchar_t *)malloc( sizeof(wchar_t)*(wcsLen + 1) );
    MultiByteToWideChar(CP_UTF8, 0,( CHAR *) bufData, ENCRYPT_BUFFER_LENGTH1, wszString, wcsLen);
    wszString[wcsLen] = '\0';

    //Change unicode string to ansi
    textlen = WideCharToMultiByte( CP_ACP, 0, ( WCHAR *)wszString, -1, NULL, 0, NULL, NULL );
    WideCharToMultiByte( CP_ACP, 0, wszString, -1, (CHAR*)bufData, textlen, NULL, NULL );

    SAFE_FREE(wszString);
}

/****************************************************************************
 * helper function: dumps a given block of data, in hex
 */

void dump_hex(unsigned char *data, unsigned int datalen, char *margin)
{
    unsigned int i;

    for (i = 0; i < datalen; i++)
    {
        if (((i % DUMP_SPACE_COLS) == 0) && (i != 0))
            printf(" ");

        if ((i % DUMP_COLS_PER_ROW) == 0)
        {
            if (i != 0)
                printf("\n");

            if (margin != NULL)
                printf("%s", margin);
        }

        /* dump character in hex */
        printf("%02X ", data[i]);
    }
} /* dump_hex */

/****************************************************************************
 * helper function: dumps a given block of data, in ascii
 */

void dump_ascii(unsigned char *data, unsigned int datalen, char *margin)
{
    unsigned int i;

    for (i = 0; i < datalen; i++)
    {
        if (((i % DUMP_SPACE_COLS) == 0) && (i != 0))
            printf(" ");

        if ((i % DUMP_COLS_PER_ROW) == 0)
        {
            if (i != 0)
                printf("\n");

            if (margin != NULL)
                printf("%s", margin);
        }

        /* dump printable character in ascii */
        printf("%c", ((data[i] > 31) && (data[i] < 128)) ? data[i] : '.');
    }
} /* dump_ascii */

/****************************************************************************
 * helper function: dumps a given block of data, in hex and ascii
 */

void dump(unsigned char *data, unsigned int datalen, char *margin)
{
    unsigned int i, icols;

    for (i = 0; i < datalen; )
    {
        icols = datalen - i;

        if (icols > DUMP_COLS_PER_ROW)
            icols = DUMP_COLS_PER_ROW;

        dump_hex(&data[i], icols, margin);
        dump_ascii(&data[i], icols, "  ");
        printf("\n");

        i += icols;
    }
} /* dump */

