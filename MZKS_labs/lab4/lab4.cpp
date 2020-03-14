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

enum class ProgramMode{EmbedMode = 0, RetrieveMode, CheckMode};

typedef struct tagBMP
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	PRGBTRIPLE bitmap;
	PBYTE pWriter;
	PBYTE pReader;
}BMP, *PBMP;

PBMP LoadBMPFromFile(LPCWSTR lpszPathToBMP)
{
	HANDLE hFileBMP;
	DWORD dwReadedByte;
	LONG biWidthBMP, biHeightBMP;
	PBMP bmp;

	/*Выделяем память под структуру BMP*/
	bmp = (PBMP)malloc(sizeof(BMP));
	if (bmp == NULL)
	{
		return FALSE;
	}

	/*Открываем файл с картинкой на чтение и проверяем открылся ли*/
	hFileBMP = CreateFile(lpszPathToBMP, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileBMP == INVALID_HANDLE_VALUE)
	{
		//printf("Error CreateFile! Error = %ld\n", GetLastError());
		return NULL;
	}

	/*Читаем из файла заголовки в структуру BMP и проверяем прочитались ли*/
	if (!ReadFile(hFileBMP, &(bmp->fileHeader), sizeof(BITMAPFILEHEADER), &dwReadedByte, NULL))
	{
		return NULL;
	}
	if (!ReadFile(hFileBMP, &(bmp->infoHeader), sizeof(BITMAPINFOHEADER), &dwReadedByte, NULL))
	{
		return NULL;
	}

	biWidthBMP = bmp->infoHeader.biWidth;
	biHeightBMP = bmp->infoHeader.biHeight;

	/*Устанавливаем указатель в файле на место, где находятся пиксели*/
	SetFilePointer(hFileBMP, bmp->fileHeader.bfOffBits, NULL, FILE_BEGIN);

	/*Выделяем память под bitmap(массив пикселей) и проверяем выделилось ли*/
	bmp->bitmap = (PRGBTRIPLE)malloc(sizeof(RGBTRIPLE) * biWidthBMP * biHeightBMP);
	if (bmp->bitmap == NULL)
	{
		return FALSE;
	}

	/*Устанавливаем указатели писателя и читателя (нужно для записи и чтения из файла bmp)*/
	bmp->pWriter = (PBYTE)bmp->bitmap;
	bmp->pReader = (PBYTE)bmp->bitmap;

	/*В цикле читаем по пикселю и сохраняем в память*/
	for (INT i = 0; i < biHeightBMP; i++)
	{
		for (INT j = 0; j < biWidthBMP; j++)
		{
			if (!ReadFile(hFileBMP, (bmp->bitmap + (DWORD)(i * biWidthBMP + j)), sizeof(RGBTRIPLE), &dwReadedByte, NULL))
			{
				return NULL;
			}
		}
		SetFilePointer(hFileBMP, biWidthBMP % 4, NULL, FILE_CURRENT);
	}

	CloseHandle(hFileBMP);

	return bmp;
}
BOOL WriteBMPToFile(LPCWSTR lpszPathToFile, PBMP bmp)
{
	HANDLE hFile;
	DWORD dwWrittedByte;
	LONG biWidthBMP, biHeightBMP;
	BYTE carryByte[4];

	/*Зануляем массив carryByte. Данный массив нужен чтобы дописывать в файл нули*/
	memset(carryByte, 0, sizeof(BYTE) * 4);

	/*Открываем файл с картинкой на запись. При этом содержимое файла теряется так как будем перезаписывать*/
	hFile = CreateFile(lpszPathToFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Error while oppening file!\n");
		return FALSE;
	}

	biWidthBMP = bmp->infoHeader.biWidth;
	biHeightBMP = bmp->infoHeader.biHeight;
	
	/*Пишем в файл заголовки из bmp структуры и проверям записались ли*/
	if (!WriteFile(hFile, &(bmp->fileHeader), sizeof(BITMAPFILEHEADER), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}
	if (!WriteFile(hFile, &(bmp->infoHeader), sizeof(BITMAPINFOHEADER), &dwWrittedByte, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}

	/*Пишем в файл массив пикселей*/
	for (INT i = 0; i < biHeightBMP; i++)
	{
		for (INT j = 0; j < biWidthBMP; j++)
		{
			if (!WriteFile(hFile, (bmp->bitmap + (i * biWidthBMP + j)), sizeof(RGBTRIPLE), &dwWrittedByte, NULL))
			{
				printf("Cannot write in file!\n");
				return FALSE;
			}
		}
		if (!WriteFile(hFile, &carryByte, biWidthBMP % 4, &dwWrittedByte, NULL))
		{
			printf("Cannot write in file!\n");
			return FALSE;
		}
	}

	CloseHandle(hFile);

	return TRUE;
}

VOID WriteBytesInBMP(PVOID pInBuffer, UINT lengthBuffer, PBMP bmp)
{
	PBYTE pBuffer = (PBYTE)pInBuffer;
	for (UINT k = 0; k < lengthBuffer; k++)
	{
		for (INT i = 0; i < 8; i++)
		{
			*(bmp->pWriter) = ((*pBuffer >> i) & 1) ? *(bmp->pWriter) | 1 : *(bmp->pWriter) & 254;
			bmp->pWriter++;
		}
		pBuffer++;
	}
}
VOID ReadBytesFromBMP(PVOID pOutBuffer, UINT lengthBuffer, PBMP bmp)
{
	PBYTE pBuffer = (PBYTE)pOutBuffer;
	for (UINT k = 0; k < lengthBuffer; k++)
	{
		for (INT i = 0; i < 8; i++)
		{
			*pBuffer = (*(bmp->pReader) & 1) ? *pBuffer | (1 << i) : *pBuffer & ~(1 << i);
			bmp->pReader++;
		}
		pBuffer++;
	}
}

BOOL EmbedMessageInBMP(LPCWSTR lpszPathToMessage, PBMP bmp)
{
	HANDLE hFile;
	DWORD dwReadedByte;
	DWORD dwCRC;
	LONG biWidthBMP, biHeightBMP;
	PBYTE pBuffer;

	/*Открываем файл с сообщением и проверяем открылся ли он*/
	hFile = CreateFile(lpszPathToMessage, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//printf("Error CreateFile! Error = %ld\n", GetLastError());
		return NULL;
	}

	/*Получаем длину файла и проверяем помещается ли сообщение в bmp*/
	DWORD dwSizeH = 0, dwSizeL = 0;
	dwSizeL = GetFileSize(hFile, &dwSizeH);
	LONGLONG fileLength = ((LONGLONG)dwSizeH * ((LONGLONG)MAXDWORD + 1)) + dwSizeL;
	if (fileLength >= MAXDWORD)
	{
		return FALSE;
	}

	biWidthBMP = bmp->infoHeader.biWidth;
	biHeightBMP = bmp->infoHeader.biHeight;

	if (biWidthBMP * biHeightBMP * 3 / 8 < fileLength + sizeof(DWORD) + sizeof(DWORD))
	{
		return FALSE;
	}

	/*Выделяем память под буфер и читаем в него содержимое файла*/
	pBuffer = (PBYTE)malloc(sizeof(BYTE) * fileLength);
	if (pBuffer == NULL)
	{
		return FALSE;
	}

	if (!ReadFile(hFile, pBuffer, (DWORD)fileLength, &dwReadedByte, NULL))
	{
		return FALSE;
	}

	if (dwReadedByte == 0)
	{
		return FALSE;
	}

	/*Вычисляем контрольную сумму сообщения*/
	dwCRC = RtlCrc32(pBuffer, fileLength, 0);

	/*Последовательно записываем контрольную сумму, размер сообщения и сообщение*/
	WriteBytesInBMP(&dwCRC, sizeof(DWORD), bmp);
	WriteBytesInBMP(&fileLength, sizeof(DWORD), bmp);
	WriteBytesInBMP(pBuffer, (DWORD)fileLength, bmp);

	return TRUE;
}
BOOL RetrieveMessageFromBMP(PBMP bmp, PVOID *pOutBuffer, PDWORD dwOutReaded)
{
	DWORD dwCRC, dwCheckCRC;
	LONG biWidthBMP, biHeightBMP;
	DWORD readedBytes;
	PBYTE pBuffer;

	/*Читаем из bmp контрольную сумму и размер сообщения*/
	ReadBytesFromBMP(&dwCRC, sizeof(DWORD), bmp);
	ReadBytesFromBMP(&readedBytes, sizeof(DWORD), bmp);

	biWidthBMP = bmp->infoHeader.biWidth;
	biHeightBMP = bmp->infoHeader.biHeight;

	/*Проверям, могло ли сообщение поместить в bmp*/
	if (biWidthBMP * biHeightBMP * 3 / 8 < readedBytes + sizeof(DWORD) + sizeof(DWORD))
	{
		return FALSE;
	}

	/*Выделяем память под буфер и читаем в него все байта сообщения из bmp*/
	pBuffer = (PBYTE)malloc(sizeof(BYTE) * readedBytes);
	if (pBuffer == NULL)
	{
		return FALSE;
	}

	ReadBytesFromBMP(pBuffer, (DWORD)readedBytes, bmp);

	/*Вычисляем контрольную сумму прочитанного сообщения*/
	dwCheckCRC = RtlCrc32(pBuffer, readedBytes, 0);

	/*Если контрольные суммы сошлись, то сообщение успешно прочитано.
	Если нет, то сообщение повреждено или не содержалось в файле*/
	if (dwCRC != dwCheckCRC)
	{
		return FALSE;
	}

	/*Копируем прочитанное сообщение в буфер назначения*/
	//memcpy_s(pOutBuffer, lengthBuffer, pBuffer, readedBytes);
	*pOutBuffer = pBuffer;
	*dwOutReaded = readedBytes;

	return TRUE;
}

INT wmain(INT argc, PWSTR argv[])
{
	PWSTR lpszPathToSecretFile = 0;
	PWSTR lpszPathToBMP = 0;
	PBMP pBMP;
	PBYTE pBuffer;
	DWORD dwReaded;
	ProgramMode mode = ProgramMode::EmbedMode;

	if (argc < 2)
	{
		printf("Expected params! Use -help \n\n");
		return 0;
	}

	/*Анализируем входные параметры*/
	for (INT i = 1; i < argc; i++)
	{
		if (wcscmp(L"-help", argv[i]) == 0)
		{
			printf("Help:\n\
\t-help - get help!\n\
\t-embed <PathToSecretMessage> <PathToBMP> - embed secret message from file in bmp\n\
\t-retrieve <PathToBMP> <PathToFile> - retrieve secret message from BMP and copy in file\n\
\t-check <PathToBMP> - check contains secret message in bmp\n");
			return 0;
		}
		else if (wcscmp(L"-embed", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			lpszPathToSecretFile = argv[i];

			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			lpszPathToBMP = argv[i];

			mode = ProgramMode::EmbedMode;
			break;
		}
		else if (wcscmp(L"-retrieve", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			lpszPathToBMP = argv[i];

			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			lpszPathToSecretFile = argv[i];

			mode = ProgramMode::RetrieveMode;
			break;
		}
		else if (wcscmp(L"-check", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			lpszPathToBMP = argv[i];

			mode = ProgramMode::CheckMode;
			break;
		}
		else
		{
			printf("Unexpected param! Try use \"-help\"\n");
			return 0;
		}
	}

	/*Открываем bmp и загружаем его в структуру*/
	if (lpszPathToBMP == NULL)
	{
		return -1;
	}
	pBMP = LoadBMPFromFile(lpszPathToBMP);
	if (pBMP == NULL)
	{
		printf("Cannot open/read BMP file!\n");
		return 0;
	}

	/*Дешефрируем работу программы*/
	switch (mode)
	{
		/*Внедряем сообщение в bmp*/
	case ProgramMode::EmbedMode:
		if (!EmbedMessageInBMP(lpszPathToSecretFile, pBMP))
		{
			printf("Cannot embed message to BMP file!\n");
			return 0;
		}

		if (!WriteBMPToFile(lpszPathToBMP, pBMP))
		{
			printf("Cannot rewrite BMP file!\n");
			return 0;
		}

		printf("Done!\n");
		break;

		/*Пытааемся прочитать сообщение из bmp*/
	case ProgramMode::RetrieveMode:
		if (!RetrieveMessageFromBMP(pBMP, (PVOID*)&pBuffer, &dwReaded))
		{
			printf("Cannot retrieve message from BMP file!\n");
			return 0;
		}
		HANDLE hFile;
		if (lpszPathToSecretFile == NULL)
		{
			return 0;
		}
		hFile = CreateFile(lpszPathToSecretFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Cannot open file %ws\n", lpszPathToSecretFile);
			return 0;
		}

		if (!WriteFile(hFile, pBuffer, dwReaded, NULL, NULL))
		{
			printf("Cannot write message in file %ws\n", lpszPathToSecretFile);
			return 0;
		}

		printf("Done!\n");
		break;

		/*Проверяем есть ли в bmp сообщение*/
	case ProgramMode::CheckMode:
		if (!RetrieveMessageFromBMP(pBMP, (PVOID*)&pBuffer, &dwReaded))
		{
			printf("BMP file dont have message!!\n");
			return 0;
		}
		printf("BMP file have message!\n");
		break;
	default:
		printf("Unxpected error! Exiting\n");
		return -1;
		break;
	}

	return 0;
}