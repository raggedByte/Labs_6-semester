#pragma once
#include "windows.h"
#include "stdio.h"

CONST UINT key[] = { 1, 2, 10, 4, 8, 7, 3, 6, 9, 5 };
CONST UINT SIZE_BLOCK = (sizeof(key) / sizeof(int));

VOID EncryptBlock(PBYTE block);
VOID DecryptBlock(PBYTE block);

BOOL encryptFile(LPCWSTR fileName);
BOOL decryptFile(LPCWSTR fileName);
