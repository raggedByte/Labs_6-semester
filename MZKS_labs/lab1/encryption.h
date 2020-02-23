#pragma once
#include "windows.h"
#include "stdio.h"

const unsigned int key[] = { 1, 2, 10, 4, 8, 7, 3, 6, 9, 5 };
const unsigned int SIZE_BLOCK = (sizeof(key) / sizeof(int));

DWORD EncryptBlock(PBYTE block);
VOID DecryptBlock(PBYTE block);
BOOL encryptFile(LPCWSTR fileName);
BOOL decryptFile(LPCWSTR fileName);
