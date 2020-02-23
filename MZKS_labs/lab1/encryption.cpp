#include "encryption.h"

DWORD EncryptBlock(PBYTE block)
{
	DWORD lengthBlock = strlen((char*)block);
	BYTE tempBlock[SIZE_BLOCK + 1] = "";

	if (lengthBlock != SIZE_BLOCK)
	{
		for (UINT i = 0; i < SIZE_BLOCK - lengthBlock; i++)
		{
			block[i + lengthBlock] = ' ';
		}
	}

	for (UINT i = 0; i < SIZE_BLOCK; i++)
	{
		tempBlock[i] = block[key[i] - 1];
	}

	memcpy_s(block, SIZE_BLOCK + 1, tempBlock, SIZE_BLOCK + 1);

	return SIZE_BLOCK - lengthBlock;
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
	DWORD dwCountTacts = 0, dwOffset = 0;
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

	SetFilePointer(hDestFile, sizeof(DWORD) * 2, NULL, FILE_BEGIN);

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
			dwOffset = EncryptBlock(buffer);
			if (!WriteFile(hDestFile, buffer, SIZE_BLOCK, &dwWrote, NULL))
			{
				printf("Error WriteFile! error = %ld\n", GetLastError());
				return FALSE;
			}
			dwCountTacts++;
		}
	} while (bResult && dwReaded);

	SetFilePointer(hDestFile, 0, NULL, FILE_BEGIN);

	if (!WriteFile(hDestFile, &dwCountTacts, sizeof(DWORD), &dwWrote, NULL))
	{
		printf("Error WriteFile! error = %ld\n", GetLastError());
		return FALSE;
	}
	if (!WriteFile(hDestFile, &dwOffset, sizeof(DWORD), &dwWrote, NULL))
	{
		printf("Error WriteFile! error = %ld\n", GetLastError());
		return FALSE;
	}

	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);

	return TRUE;
}

BOOL decryptFile(LPCWSTR fileName)
{
	HANDLE hSourceFile, hDestFile;
	DWORD dwReaded = 0, dwWrote = 0;
	DWORD dwCountTacts, dwOffset;
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

	if (!ReadFile(hSourceFile, &dwCountTacts, sizeof(DWORD), &dwReaded, NULL))
	{
		printf("Error ReadFile! error = %ld\n", GetLastError());
		return FALSE;
	}
	if (!ReadFile(hSourceFile, &dwOffset, sizeof(DWORD), &dwReaded, NULL))
	{
		printf("Error ReadFile! error = %ld\n", GetLastError());
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
			DecryptBlock(buffer);
			if (--dwCountTacts == 0)
			{
				if (!WriteFile(hDestFile, buffer, SIZE_BLOCK - dwOffset, &dwWrote, NULL))
				{
					printf("Error WriteFile! error = %ld\n", GetLastError());
					return FALSE;
				}
			}
			else
			{
				if (!WriteFile(hDestFile, buffer, SIZE_BLOCK, &dwWrote, NULL))
				{
					printf("Error WriteFile! error = %ld\n", GetLastError());
					return FALSE;
				}
			}
		}
	} while (bResult && dwReaded);

	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);

	return TRUE;
}
