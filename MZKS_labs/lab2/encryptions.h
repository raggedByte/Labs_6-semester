#pragma once
#include "stdio.h"
#include "windows.h"

typedef UINT BLOCK;
typedef UINT BIT;

CONST UINT key[] = { 12, 13, 30, 5, 27, 6, 11, 25, 3, 21, 22, 2, 23, 0, 8, 4, 18, 19, 10, 1, 14, 29, 9, 28, 20, 17, 26, 31, 7, 16, 15, 24 };
CONST UINT LENGTH_KEY = (sizeof(key) / sizeof(UINT));
CONST UINT SIZE_BLOCK = sizeof(BLOCK);

BLOCK EncryptBlock(BLOCK block);
BLOCK DecryptBlock(BLOCK block);

BIT GetBit(BLOCK block, INT numBit);
BIT SetBit(BLOCK block, INT numBit);

BOOL encryptFile(LPCWSTR fileName);
BOOL decryptFile(LPCWSTR fileName);