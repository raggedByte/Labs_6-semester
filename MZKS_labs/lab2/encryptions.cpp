#include "encryptions.h"

BOOL encryptFile(LPCWSTR fileName)
{
	HANDLE hSourceFile, hDestFile;
	DWORD dwReaded = 0, dwWrote = 0;
	BLOCK block = 0;
	BOOL bResult = FALSE;

	hSourceFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSourceFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return FALSE;
	}

	hDestFile = CreateFile(TEXT("encrypted"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDestFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return -1;
	}

	DWORD dwSizeH = 0, dwSizeL = 0;
	dwSizeL = GetFileSize(hSourceFile, &dwSizeH);
	LONGLONG fileLength = ((LONGLONG)dwSizeH * ((LONGLONG)MAXDWORD + 1)) + dwSizeL;

	if (!WriteFile(hDestFile, &fileLength, sizeof(LONGLONG), &dwWrote, NULL))
	{
		printf("Error WriteFile! error = %ld\n", GetLastError());
		return FALSE;
	}

	do
	{
		memset(&block, 0, SIZE_BLOCK);
		if (!(bResult = ReadFile(hSourceFile, &block, SIZE_BLOCK, &dwReaded, NULL)))
		{
			printf("Error ReadFile! error = %ld\n", GetLastError());
			return FALSE;
		}
		if (dwReaded)
		{
			block = EncryptBlock(block);
			if (!WriteFile(hDestFile, &block, SIZE_BLOCK, &dwWrote, NULL))
			{
				printf("Error WriteFile! error = %ld\n", GetLastError());
				return FALSE;
			}
		}
	} while (bResult && dwReaded);

	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);

	return TRUE;
}
BOOL decryptFile(LPCWSTR fileName)
{
	HANDLE hSourceFile, hDestFile;
	DWORD dwReaded = 0, dwWrote = 0;
	BLOCK block = 0;

	hSourceFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSourceFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return FALSE;
	}

	hDestFile = CreateFile(TEXT("decrypted"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDestFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return -1;
	}

	LONGLONG sourceLength;

	if (!ReadFile(hSourceFile, &sourceLength, sizeof(LONGLONG), &dwReaded, NULL))
	{
		printf("Error ReadFile! error = %ld\n", GetLastError());
		return FALSE;
	}

	while (sourceLength > SIZE_BLOCK)
	{
		sourceLength -= SIZE_BLOCK;
		memset(&block, 0, SIZE_BLOCK);
		if (!ReadFile(hSourceFile, &block, SIZE_BLOCK, &dwReaded, NULL))
		{
			printf("Error ReadFile! error = %ld\n", GetLastError());
			return FALSE;
		}
		if (dwReaded)
		{
			block = DecryptBlock(block);
			if (!WriteFile(hDestFile, &block, SIZE_BLOCK, &dwWrote, NULL))
			{
				printf("Error WriteFile! error = %ld\n", GetLastError());
				return FALSE;
			}
		}
	}

	memset(&block, 0, SIZE_BLOCK);
	if (!ReadFile(hSourceFile, &block, SIZE_BLOCK, &dwReaded, NULL))
	{
		printf("Error ReadFile! error = %ld\n", GetLastError());
		return FALSE;
	}
	block = DecryptBlock(block);
	if (!WriteFile(hDestFile, &block, (DWORD)sourceLength, &dwWrote, NULL))
	{
		printf("Error WriteFile! error = %ld\n", GetLastError());
		return FALSE;
	}

	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);

	return TRUE;
}

BLOCK EncryptBlock(BLOCK block)
{
	BLOCK tempBlock = 0;

	for (UINT i = 0; i < LENGTH_KEY; i++)
	{
		BIT tempBit = GetBit(block, key[i]);
		if (tempBit == 1)
			tempBlock = SetBit(tempBlock, i);
	}

	return tempBlock;
}
BLOCK DecryptBlock(BLOCK block)
{
	BLOCK tempBlock = 0;

	for (UINT i = 0; i < LENGTH_KEY; i++)
	{
		BIT tempBit = GetBit(block, i);
		if (tempBit == 1)
			tempBlock = SetBit(tempBlock, key[i]);
	}

	return tempBlock;
}

BIT GetBit(BLOCK block, INT numBit)
{
	return ((block & (1 << numBit)) != 0);
}
BIT SetBit(BLOCK block, INT numBit)
{
	return (block | (1 << numBit));
}