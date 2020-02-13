/*
	Разработать программу, выполняющую шифрование и
расшифровывание произвольного текстового файла с использованием
перестановочного шифра используя в качестве ключа последовательность,
соответствующую номеру варианта. Выполнить проверку путем двоичного
сравнения исходного файла и фала, полученного после расшифровывания.

	Вариант 4. Ключ - 1 2 10 4 8 7 3 6 9 5
*/

#include "windows.h"
#include <cstdio>

int key[] = { 1, 2, 10, 4, 8, 7, 3, 6, 9, 5 };
const unsigned int SIZE_BLOCK = (sizeof(key) / sizeof(int));

void cryptMessage(char* message)
{
	int length = strlen(message);
	if (length != SIZE_BLOCK)
	{
		for (int i = 0; i < SIZE_BLOCK - length; i++)
		{
			message[i + length] = ' ';
		}
	}
	char tempBuffer[SIZE_BLOCK + 1];
	tempBuffer[SIZE_BLOCK] = 0;
	for (int i = 0; i < SIZE_BLOCK; i++)
	{
		tempBuffer[i] = message[key[i] - 1];
	}
	//strcpy(message, tempBuffer);
	strcpy_s(message, SIZE_BLOCK + 1, tempBuffer);
}

void decryptMessage(char* message)
{
	int length = strlen(message);
	if (length != SIZE_BLOCK)
	{
		for (int i = 0; i < SIZE_BLOCK - length; i++)
		{
			message[i + length] = ' ';
		}
	}
	char tempBuffer[SIZE_BLOCK + 1];
	tempBuffer[SIZE_BLOCK] = 0;
	for (int i = 0; i < SIZE_BLOCK; i++)
	{
		tempBuffer[key[i] - 1] = message[i];
	}
	//strcpy(message, tempBuffer);
	strcpy_s(message, SIZE_BLOCK + 1, tempBuffer);
}

int main(int argc, char* argv[])
{
	char* sourcePath = 0, * destPath = 0;
	bool crypt = false, decrypt = false, compare = false;
	FILE* pSourceFile = 0, * pDestFile = 0;
	char buffer[SIZE_BLOCK + 1];

	if (argc <= 1)
	{
		printf("Programm was openned without params. \nUse param \"-help\" for get help\n");
		return 0;
	}
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-help", argv[i]) == 0)
		{
			printf("Help:\n\t-help - get help about params\n\t-crypt <filename> [-compare] - crypt current \
file and save it in root folder\n\t-decrypt <filename> - decrypt current file and save it in root folder\n");
			return 0;
		}
		else
			if (strcmp("-crypt", argv[i]) == 0)
			{
				if (i + 1 >= argc)
				{
					printf("Expected file path for crypt! Try use \"-help\n");
					return 0;
				}
				i++;
				__try
				{
					sourcePath = (char*)malloc(sizeof(char) * strlen(argv[i]) + 1);
					if (sourcePath == NULL)
					{
						printf("Program cannot alloc memory!\n");
						return -1;
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					printf("Was met exception! Code exception = %ld\n", GetExceptionCode());
					return -1;
				}
				//strcpy(sourcePath, argv[i]);
				strcpy_s(sourcePath, strlen(argv[i]) + 1, argv[i]);
				crypt = true;
			}
			else
				if (strcmp("-decrypt", argv[i]) == 0)
				{
					if (i + 1 >= argc)
					{
						printf("Expected file path for decrypt! Try use \"-help\n");
						return 0;
					}
					i++;
					__try
					{
						destPath = (char*)malloc(sizeof(char) * strlen(argv[i]) + 1);
						if (destPath == NULL)
						{
							printf("Program cannot alloc memory!\n");
							return -1;
						}
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						printf("Was met exception! Code exception = %ld\n", GetExceptionCode());
						return -1;
					}
					//strcpy(destPath, argv[i]);
					strcpy_s(destPath, strlen(argv[i]) + 1, argv[i]);

					decrypt = true;
				}
				else
					if (strcmp("-compare", argv[i]))
					{
						compare = true;
					}
					else
					{
						printf("Unexpected param! Try use \"-help\"\n");
						return 0;
					}
	}

	printf("Lab 1. Crypt and decrypt text.\n\n");

	if (crypt)
	{
		fopen_s(&pSourceFile, sourcePath, "r+");
		if (pSourceFile == 0)
		{
			printf("Cannot open file! Check source path in param! \nTry use \"-help\"\n");
			return 0;
		}
		fopen_s(&pDestFile, "crypted.txt", "w");
		if (pDestFile == 0)
		{
			printf("Cannot create file! Check source path in param! \nTry use \"-help\"\n");
			return 0;
		}
		while (!feof(pSourceFile))
		{
			memset(buffer, 0, SIZE_BLOCK + 1);
			fread(buffer, 1, SIZE_BLOCK, pSourceFile);
			if (strlen(buffer) == 0)
				break;
			cryptMessage(buffer);
			fprintf(pDestFile, "%s", buffer);
		}
		printf("File %s was crypted! Check new file \"crypted.txt\"\n", sourcePath);
		fclose(pSourceFile);
		fclose(pDestFile);
	}
	if (decrypt)
	{
		fopen_s(&pSourceFile, destPath, "r+");
		if (pSourceFile == NULL)
		{
			printf("Cannot open file! Check source path in param! \nTry use \"-help\"\n");
			return 0;
		}
		fopen_s(&pDestFile, "decrypted.txt", "w");
		if (pDestFile == NULL)
		{
			printf("Cannot create file! \nTry use \"-help\"\n");
			return 0;
		}
		while (!feof(pSourceFile))
		{
			memset(buffer, 0, SIZE_BLOCK + 1);
			fread(buffer, 1, SIZE_BLOCK, pSourceFile);
			if (strlen(buffer) == 0)
				break;
			decryptMessage(buffer);
			fprintf(pDestFile, "%s", buffer);
		}
		printf("File %s was decrypted! Check new file \"decrypted.txt\"\n", destPath);
		fclose(pSourceFile);
		fclose(pDestFile);
	}
	if (compare)
	{
		if (!crypt)
		{
			printf("Param \"-compare\" should used with param \"-crypt\"!\n");
			return 0;
		}
		/*compare here*/
	}
	return 0;
}