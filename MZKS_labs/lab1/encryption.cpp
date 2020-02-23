#include "encryption.h"

VOID EncryptBlock(PBYTE block)
{
	DWORD lengthBlock = strlen((char*)block);
	BYTE tempBlock[SIZE_BLOCK + 1] = "";

	/*if (lengthBlock != SIZE_BLOCK)
	{
		for (UINT i = 0; i < SIZE_BLOCK - lengthBlock; i++)
		{
			block[i + lengthBlock] = ' ';
		}
	}*/

	for (UINT i = 0; i < SIZE_BLOCK; i++)
	{
		tempBlock[i] = block[key[i] - 1];
	}

	memcpy_s(block, SIZE_BLOCK + 1, tempBlock, SIZE_BLOCK + 1);
}

VOID DecryptBlock(PBYTE block)
{
	BYTE tempBlock[SIZE_BLOCK + 1] = "";
	for (UINT i = 0; i < SIZE_BLOCK; i++)
	{
		tempBlock[key[i] - 1] = block[i];
	}

	memcpy_s(block, SIZE_BLOCK + 1, tempBlock, SIZE_BLOCK + 1);
}

BOOL encryptFile(LPCWSTR fileName)
{
	HANDLE hSourceFile, hDestFile;
	DWORD dwReaded = 0, dwWrote = 0;
	BYTE buffer[SIZE_BLOCK + 1];
	BOOL bResult = FALSE;

	hSourceFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSourceFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return FALSE;
	}

	hDestFile = CreateFile(TEXT("encrypted.txt"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
		memset(buffer, 0, SIZE_BLOCK + 1);
		if (!(bResult = ReadFile(hSourceFile, buffer, SIZE_BLOCK, &dwReaded, NULL)))
		{
			printf("Error ReadFile! error = %ld\n", GetLastError());
			return FALSE;
		}
		if (dwReaded)
		{
			EncryptBlock(buffer);
			if (!WriteFile(hDestFile, buffer, SIZE_BLOCK, &dwWrote, NULL))
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
	BYTE buffer[SIZE_BLOCK + 1];
	BOOL bResult = FALSE;

	hSourceFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSourceFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return FALSE;
	}

	hDestFile = CreateFile(TEXT("decrypted.txt"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
		memset(buffer, 0, SIZE_BLOCK + 1);
		if (!(bResult = ReadFile(hSourceFile, buffer, SIZE_BLOCK, &dwReaded, NULL)))
		{
			printf("Error ReadFile! error = %ld\n", GetLastError());
			return FALSE;
		}
		if (dwReaded)
		{
			DecryptBlock(buffer);
			if (!WriteFile(hDestFile, buffer, SIZE_BLOCK, &dwWrote, NULL))
			{
				printf("Error WriteFile! error = %ld\n", GetLastError());
				return FALSE;
			}
		}
	}

	memset(buffer, 0, SIZE_BLOCK + 1);
	if (!(bResult = ReadFile(hSourceFile, buffer, SIZE_BLOCK, &dwReaded, NULL)))
	{
		printf("Error ReadFile! error = %ld\n", GetLastError());
		return FALSE;
	}
	DecryptBlock(buffer);
	if (!WriteFile(hDestFile, buffer, sourceLength, &dwWrote, NULL))
	{
		printf("Error WriteFile! error = %ld\n", GetLastError());
		return FALSE;
	}

	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);

	return TRUE;
}
