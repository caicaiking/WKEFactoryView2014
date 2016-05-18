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

        "FYqLT8uT6HadrrztaO1WVyZ3IOEtBqqIX4unLZFEMmUnyuu9WfcAQMIxpNG60vfBXDpLCUn2Aq"
        "gro8oB/Ds0Q8DNzrtF0XUhXzPiMdwuCaNL20udvhRFMaR63LeAFX4tCX8V+WHURb0MqMIIRdcX"
        "Mhxv3D/MUZ2wiUVjxe71Wgr9/16U46gU5XZlLM2XAUDySgAYKzF0JQ7X6duj2uh78TVgCR6+J1"
        "ZWwEJvpPFhbQDBdTpUkpnyfX+ASBmtCHan66LbiS+OyYvgG8jdOrHXe/OgWuR3gAS9rGCFM/0n"
        "by6D9Ops4rBls5kxuWbLIi+oFUpq1tSaGOdoRpfLho+derZ4ouKYMahF7J4KbfjCm8OTTNJZc+"
        "N8C4j9gGZjC4ZKTubTKQYTYxe5n6DBwlxw5jsLhMpCOGpAaZ4wywOCTOD0JhGD+dImDgGB9/ex"
        "CMgqw3aJhW9YJ4GSD9W4UPVllTL66+vB8p5CnocP5TPopUVtzhN9gjHhxh8R+i7UzIRJuIyPNB"
        "WzBNr/CbUTL+AJ54HwGPnyLNirHNxdvSysCSoh+UpE8SBgIQWowZu0uW2YWvh+Xalb+W596yNJ"
        "3FDAS8hiIVSrP1fTRzq3c0HVoGweoKWWzuLb8SdFDBahX9jD3/H8CvUGCwqfiQhpoeJ3JWJig0"
        "ZmCcC3rC8mUfutaDVoFq6Ww2R0onoiwYQOAo6bN9qk0+WO96QrnY51Hd6KBY5JAsF7YRGYe9ft"
        "77muSAgwkYeKnBFSeuKMJL9DI8SOItZIwqoXFdp2Y8WgKh6DgdzlqyiDfo3W5f+jNDRQyKTWVa"
        "cM9nWHXXVhWa5Fd+cjSu4Le+MXmpiTtfEoljNPukvMcSCgqpMf8NfW3bzrxC68njRdoaNeUzG6"
        "ufeiv7MJp12QVhXXAPzEZ5WmB12bj5A0aBAVQe6Q04HgR0Y/Q9XL+N8aJpJmME6AXHoqUcU7iu"
        "3K7p8iQyyN5wrV6FnXFw==";

//WKE FactoryView 2014
#define ENCRYPT_BUFFER_FEATUREID1 1    //feature id which is selected
#define SOURCE_BUFFER_LENGTH1 20    //length of source string
#define ENCRYPT_BUFFER_LENGTH1 20    //length of encrypt string

const int isString1 = 1;    //This is a string buffer
/*The encrypted array is in UTF-8 format. Please convert it to proper format before using it.*/

const unsigned char encryptStrArr1[20] = {
   0x38, 0x20, 0xC1, 0x28, 0x8B, 0xD9, 0x20, 0x8C, 0xF2, 0x55, 0x5B, 0x15, 0x28, 0xBC, 0x71, 0x52, 0x9C, 0xFC, 0xED, 0x79
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
