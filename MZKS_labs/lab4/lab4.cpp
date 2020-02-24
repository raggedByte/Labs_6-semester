/*
	Разработать программу, выполняющую внедрение, извлечение и
проверку наличия некоторых данных в файле с изображением (формат файла
с изображением — BMP, не содержащий плитру и имеющий значение
biBitCount равное 24).
	Тип операции (внедрение, извлечение и проверка), а также имена файлов,
участвующих в каждой операции следует передавать в программу через
командную строку.
*/

#include "cstdio"
#include "windows.h"

typedef struct sBMP
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	LPRGBQUAD palette;
	PRGBTRIPLE bitmap;
	sBMP()
	{
		palette = 0;
		bitmap = 0;
	}
	~sBMP()
	{
		if (palette)
			free(palette);
		if (bitmap)
			free(bitmap);
	}
}*pBMP, BMP;

BOOL LoadBitmapFromFile (pBMP hBMP, LPCWSTR lpszFile)
{
	HANDLE hFile;
	DWORD dwReadedByte;
	BOOL fResult;
	LONG biWidth, biHeight;
	BYTE carryByte[4];

	hFile = CreateFile(lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Error CreateFile! Error = %ld\n", GetLastError());
		return FALSE;
	}

	memset(hBMP, 0, sizeof(BMP));

	if (!ReadFile(hFile, &(hBMP->fileHeader), sizeof(BITMAPFILEHEADER), &dwReadedByte, NULL))
	{
		printf("Error ReadFile!\n");
		return FALSE;
	}
	if (!ReadFile(hFile, &(hBMP->infoHeader), sizeof(BITMAPINFOHEADER), &dwReadedByte, NULL))
	{
		printf("Error ReadFile!\n");
		return FALSE;
	}

	/*
	if (hBMP->infoHeader.biBitCount < 16)
	{
		hBMP->palette = (LPRGBQUAD)malloc(sizeof(RGBQUAD) * 1024);
		if (hBMP->palette == 0)
		{
			printf("Error allocation memory!\n");
			return FALSE;
		}
		if (!ReadFile(hFile, (hBMP->palette), 1024, &dwReadedByte, NULL))
		{
			printf("Error ReadFile!\n");
			return FALSE;
		}
	}
	else
	{
		hBMP->palette = 0;
	}*/

	biWidth = hBMP->infoHeader.biWidth;
	biHeight = hBMP->infoHeader.biHeight;

	SetFilePointer(hFile, hBMP->fileHeader.bfOffBits, NULL, FILE_BEGIN);

	hBMP->bitmap = (PRGBTRIPLE)malloc(sizeof(RGBTRIPLE) * biWidth * biHeight);
	if (hBMP->bitmap == 0)
	{
		printf("Error allocation memory!\n");
		return FALSE;
	}

	for (INT i = 0; i < biHeight; i++)
	{
		for (INT j = 0; j < biWidth; j++)
		{
			if (!ReadFile(hFile, (hBMP->bitmap + (i * biWidth + j)), sizeof(RGBTRIPLE), &dwReadedByte, NULL))
			{
				printf("Error ReadFile!\n");
				return FALSE;
			}
		}
		if (!ReadFile(hFile, &carryByte, sizeof(BYTE) * (biWidth % 4), &dwReadedByte, NULL))
		{
			printf("Error ReadFile!\n");
			return FALSE;
		}
	}

	CloseHandle(hFile);

	return TRUE;
}

BOOL WriteBitmapInFile(pBMP hBMP, LPCWSTR lpszFile)
{
	HANDLE hFile;
	DWORD dwWrittedByte;
	BOOL fResult;
	LONG biWidth, biHeight;
	BYTE carryByte[4];

	memset(carryByte, 0, sizeof(BYTE) * 4);

	hFile = CreateFile(lpszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Error while oppening file!\n");
		return FALSE;
	}

	biWidth = hBMP->infoHeader.biWidth;
	biHeight = hBMP->infoHeader.biHeight;

	/*
		Вставляем сообщение
	*/
	BYTE message[] = "Hi!asdasdfasdfqawefasdfgasdfgaqwergasrgfbsafssarewSGAFESDNFSARSFDBV";
	//hBMP->fileHeader.bfOffBits += sizeof(message);

	if (!WriteFile(hFile, &(hBMP->fileHeader), sizeof(BITMAPFILEHEADER), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}
	if (!WriteFile(hFile, &(hBMP->infoHeader), sizeof(BITMAPINFOHEADER), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}
	if (hBMP->palette)
	{
		if (!WriteFile(hFile, &(hBMP->palette), 1024, &dwWrittedByte, NULL))
		{
			printf("Cannot write in file!\n");
			return FALSE;
		}
	}

	for (INT i = 0; i < biHeight; i++)
	{
		for (INT j = 0; j < biWidth; j++)
		{
			if (!WriteFile(hFile, (hBMP->bitmap + (i * biWidth + j)), sizeof(RGBTRIPLE), &dwWrittedByte, NULL))
			{
				printf("Cannot write in file!\n");
				return FALSE;
			}
		}
		if (!WriteFile(hFile, &carryByte, biWidth % 4, &dwWrittedByte, NULL))
		{
			printf("Cannot write in file!\n");
			return FALSE;
		}
	}

	if (!WriteFile(hFile, message, sizeof(message), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}

	CloseHandle(hFile);

	return TRUE;
}

BOOL WriteSecretInBMP(LPCWSTR lpszFileBMP, LPCWSTR lpszFileSecret)
{
	HANDLE hFileBMP, hFileSecret;
	DWORD  dwWrittedByte, dwReadedByte;
	LONG   biWidth, biHeight;
	PBYTE  buffer;
	BYTE   carryByte[4];
	BMP    bmp;

	memset(carryByte, 0, sizeof(BYTE) * 4);

	if (!LoadBitmapFromFile(&bmp, lpszFileBMP))
	{
		return FALSE;
	}

	hFileBMP = CreateFile(lpszFileBMP, GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileBMP == INVALID_HANDLE_VALUE)
	{
		printf("Error while oppening file!\n");
		return FALSE;
	}

	hFileSecret = CreateFile(lpszFileSecret, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileSecret == INVALID_HANDLE_VALUE)
	{
		printf("Error while oppening file!\n");
		return FALSE;
	}

	DWORD dwSizeH = 0, dwSizeL = 0;
	dwSizeL = GetFileSize(hFileSecret, &dwSizeH);
	LONGLONG lengthSecretFile = ((LONGLONG)dwSizeH * ((LONGLONG)MAXDWORD + 1)) + dwSizeL;

	buffer = (PBYTE)malloc(sizeof(BYTE) * lengthSecretFile);
	if (buffer == 0)
	{
		printf("Error allocation memory!\n");
		return FALSE;
	}

	if (!ReadFile(hFileSecret, buffer, lengthSecretFile, &dwReadedByte, NULL))
	{
		printf("Error ReadFile!\n");
		return FALSE;
	}

	CloseHandle(hFileSecret);

	bmp.fileHeader.bfOffBits += lengthSecretFile;

	if (!WriteFile(hFileBMP, &(bmp.fileHeader), sizeof(BITMAPFILEHEADER), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}
	if (!WriteFile(hFileBMP, &(bmp.infoHeader), sizeof(BITMAPINFOHEADER), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}
	if (bmp.palette)
	{
		if (!WriteFile(hFileBMP, &(bmp.palette), 1024, &dwWrittedByte, NULL))
		{
			printf("Cannot write in file!\n");
			return FALSE;
		}
	}
	if (!WriteFile(hFileBMP, buffer, lengthSecretFile, &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}

	biWidth = bmp.infoHeader.biWidth;
	biHeight = bmp.infoHeader.biHeight;

	for (INT i = 0; i < biHeight; i++)
	{
		for (INT j = 0; j < biWidth; j++)
		{
			if (!WriteFile(hFileBMP, (bmp.bitmap + (i * biWidth + j)), sizeof(RGBTRIPLE), &dwWrittedByte, NULL))
			{
				printf("Cannot write in file!\n");
				return FALSE;
			}
		}
		if (!WriteFile(hFileBMP, &carryByte, biWidth % 4, &dwWrittedByte, NULL))
		{
			printf("Cannot write in file!\n");
			return FALSE;
		}
	}
	CloseHandle(hFileBMP);

	return TRUE;
}

INT wmain(INT argc, PWSTR argv[])
{
	BMP bmp;
	/*
	if (!LoadBitmapFromFile(&bmp, L"file.bmp"))
	{
		printf("GLE = %ld", GetLastError());
		return -1;
	}

	if (!WriteBitmapInFile(&bmp, L"copy.bmp"))
	{
		printf("GLE = %ld", GetLastError());
		return -1;
	}
	*/
	if (!WriteSecretInBMP(L"file.bmp", L"secret.txt"))
	{
		printf("GLE = %ld", GetLastError());
		return -1;
	}
	return 0;
}