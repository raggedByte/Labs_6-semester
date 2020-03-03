/*
	Разработать программу, выполняющую шифрование и
расшифровывание произвольного текстового файла с использованием
перестановочного шифра используя в качестве ключа последовательность,
соответствующую номеру варианта. Выполнить проверку путем двоичного
сравнения исходного файла и фала, полученного после расшифровывания.
	Вариант 4. Ключ - 1 2 10 4 8 7 3 6 9 5
*/

#include "stdio.h"
#include "encryption.h"

INT wmain(INT argc, PWSTR argv[])
{
	PWSTR lpszEncryptPath = 0;
	PWSTR lpszDecryptPath = 0;
	BOOL bCompare = FALSE;

	printf("Lab 1. Crypt and decrypt text.\n\n");

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
			bCompare = true;
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
			PWSTR commandLine = (PWSTR)malloc(sizeof(WCHAR) * 256);
			if (commandLine == 0)
			{
				return -1;
			}

			printf("\n");

			wsprintf(commandLine, L"fc %ls decrypted", lpszEncryptPath);
			_wsystem(commandLine);
		}
		else
		{
			printf("Param -compare shoud use with params -encrypt and -decrypt!\n Use -help!\n");
			return -1;
		}
	}
}