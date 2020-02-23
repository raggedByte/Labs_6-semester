﻿/*
	Разработать программу, выполняющую шифрование и
расшифровывание произвольного двоичного файла с использованием
перестановочного шифра используя в качестве ключа последовательность,
соответствующую номеру варианта. Выполнить проверку путем двоичного
сравнения исходного файла и фала, полученного после расшифровывания.
	Размер блока: 4 байта
	Вариант 4. Ключ - 12 13 30 5 27 6 11 25 3 21 22 2 23 0 8 4 18 19 10 1 14 29 9 28 20 17 26 31 7 16 15 24
*/

#include <cstdio>
#include "encryptions.h"

int wmain(INT argc, PWSTR argv[])
{
	PWSTR sEncryptPath = 0;
	PWSTR sDecryptPath = 0;
	BOOL  bCompare = false, bCheckWork = false;

	printf("\nLab 2. Crypt and decrypt text with bits encrypting.\n\n");

	if (argc < 2)
	{
		printf("Expected more params! Use -help for help . . .\n");
		return 0;
	}

	for (UINT i = 1; i < argc; i++)
	{
		if (wcscmp(TEXT("-help"), argv[i]) == 0)
		{
			printf("Help:\n\t-help - get help about params.\n\t-encrypt <filename> - encrypt current \
file and save it in root folder like \"encrypted\".\n\t-decrypt <filename> - decrypt current file and save it in root folder like \"decrypted\".\n\
\t-compare - compare two files and show difference. Should use with -encrypt and -decrypt!\n\
\t-checkWork <filename> - encrypt file, decrypt file and compare result decryption with source file.\n");
			return 0;
		}
		else if (wcscmp(TEXT("-encrypt"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected file path for encrypt! Try use \"-help\n");
				return 0;
			}
			sEncryptPath = argv[i];
		}
		else if (wcscmp(TEXT("-decrypt"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected file path for decrypt! Try use \"-help\n");
				return 0;
			}
			sDecryptPath = argv[i];
		}
		else if (wcscmp(TEXT("-compare"), argv[i]) == 0)
		{
			bCompare = true;
		}
		else if (wcscmp(TEXT("-checkWork"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected file path for decrypt! Try use \"-help\n");
				return 0;
			}
			sEncryptPath = argv[i];
			bCheckWork = true;
		}
		else
		{
			printf("Unexpected param! Try use \"-help\"\n");
			return 0;
		}
	}

	if (sEncryptPath || bCheckWork)
	{
		if (!encryptFile(sEncryptPath))
		{
			printf("Error while prorgram encrypt file! Error = %ld\n", GetLastError());
			return -1;
		}
		printf("File encrypted!\n");
	}

	if (bCheckWork)
	{
		sDecryptPath = (PWSTR)malloc(sizeof(WCHAR) * wcslen(TEXT("encrypted")) + 1);
		wsprintf(sDecryptPath, TEXT("encrypted"));
	}

	if (sDecryptPath)
	{
		if (!decryptFile(sDecryptPath))
		{
			printf("Error while prorgram decrypt file! Error = %ld\n", GetLastError());
			return -1;
		}
		printf("File decrypted!\n");
	}

	if (bCompare || bCheckWork)
	{
		if (sEncryptPath && sDecryptPath)
		{
			PWSTR commandLine = (PWSTR)malloc(sizeof(WCHAR) * 256);
			if (commandLine == 0)
			{
				return -1;
			}

			printf("\n");

			wsprintf(commandLine, L"fc %ls decrypted", sEncryptPath);
			_wsystem(commandLine);
		}
		else
		{
			printf("Param -compare should use with params -encrypt and -decrypt!\n Use -help!\n");
			return -1;
		}
	}
}