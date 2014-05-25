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
        return status;
    }


    bufData = (unsigned char *)malloc(ENCRYPT_BUFFER_LENGTH1+1);
    memset(bufData, 0, ENCRYPT_BUFFER_LENGTH1+1);

    status= dog_read(hDog,1,0,SOURCE_BUFFER_LENGTH1,bufData);
    if (DOG_STATUS_OK != status)
    {
        return status;
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

