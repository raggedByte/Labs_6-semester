/*
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

INT wmain(INT argc, PWSTR argv[])
{
	PWSTR lpszEncryptPath = 0;
	PWSTR lpszDecryptPath = 0;
	BOOL  bCompare = FALSE;

	printf("\nLab 2. Crypt and decrypt text with bits encrypting.\n\n");

	if (argc < 2)
	{
		printf("Expected more params! Use -help for help . . .\n");
		return 0;
	}

	for (INT i = 1; i < argc; i++)
	{
		if (wcscmp(TEXT("-help"), argv[i]) == 0)
		{
			printf("Help:\n\
\t-encrypt <filename> - encrypt current file and save it in root folder like \"encrypted\".\n\
\t-decrypt <filename> - decrypt current file and save it in root folder like \"decrypted\".\n\
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
			lpszEncryptPath = argv[i];
		}
		else if (wcscmp(TEXT("-decrypt"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected file path for decrypt! Try use \"-help\n");
				return 0;
			}
			lpszDecryptPath = argv[i];
		}
		else if (wcscmp(TEXT("-compare"), argv[i]) == 0)
		{
			bCompare = TRUE;
		}
		else if (wcscmp(TEXT("-checkWork"), argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected file path for decrypt! Try use \"-help\n");
				return 0;
			}
			lpszEncryptPath = argv[i];
			lpszDecryptPath = (PWSTR)malloc(sizeof(WCHAR) * wcslen(TEXT("encrypted")) + 1);
			if (lpszDecryptPath == 0)
			{
				printf("Error! Cannot alloc memory!\n");
				return -1;
			}
			wsprintf(lpszDecryptPath, TEXT("encrypted"));
			bCompare = true;
		}
		else
		{
			printf("Unexpected param! Try use \"-help\"\n");
			return 0;
		}
	}

	if (lpszEncryptPath)
	{
		if (!encryptFile(lpszEncryptPath))
		{
			printf("Error while prorgram encrypt file! Error = %ld\n", GetLastError());
			return -1;
		}
		printf("File encrypted!\n");
	}

	if (lpszDecryptPath)
	{
		if (!decryptFile(lpszDecryptPath))
		{
			printf("Error while prorgram decrypt file! Error = %ld\n", GetLastError());
			return -1;
		}
		printf("File decrypted!\n");
	}

	if (bCompare)
	{
		if (lpszEncryptPath && lpszDecryptPath)
		{
			WCHAR commandLine[256];

			printf("\n");

			wsprintf(commandLine, L"fc %ls decrypted", lpszEncryptPath);
			_wsystem(commandLine);
		}
		else
		{
			printf("Param -compare should use with params -encrypt and -decrypt!\n Use -help!\n");
			return -1;
		}
	}
}