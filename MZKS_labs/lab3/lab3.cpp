/*
	Разработать генератор случайных чисел, используя метод в соответствии с
номером варианта.Исследовать качество генератора, путем оценки распределения
генерируемых чисел.Для этого необходимо разделить весь диапазон генерируемых чисел
на 10 равных интервалов и подсчитать количество чисел, попадающих в каждый интервал.
	В качестве начального значения следует выбирать текущее время в формате Unix
time(можно получить, используя функцию time_t time(time_t* timer)).
	Для линейного конгруэнтного метода значение m всегда должно быть 2^31 - 1

	Вариант 4. Линейный конгруэнтный метод, a = 16807, c = 4
*/

#include "windows.h"
#include <cstdio>
#include <time.h>

ULONG I = 0;
CONST ULONG a = 16807;
CONST ULONG c = 4;
CONST ULONG m = 2147483648 - 1;

VOID SetRand(ULONG seed)
{
	I = seed;
}

ULONG GetRandValue()
{
	I = (a * I + c) % m;
	return I;
}

INT main(INT argc, PCHAR argv[])
{
	time_t t;
	ULONG  range = 0, seed = 0;

	if (argc < 2)
	{
		printf("Expected params! Use -help \n\n");
		return 0;
	}

	for (INT i = 1; i < argc; i++)
	{
		if (strcmp("-help", argv[i]) == 0)
		{
			printf("Help:\n\t-help - get help about params\n\t-range <value> - set range printable rand values \
\n\t-seed <value> - set new seed for rand function\n\t-randomSeed - set seed as time on computer in seconds\n");
			return 0;
		}
		else if (strcmp("-range", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			range = atoi(argv[i]);
		}
		else if (strcmp("-seed", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			seed = atoi(argv[i]);
			SetRand(seed);
		}
		else if (strcmp("-randomSeed", argv[i]) == 0)
		{
			time(&t);
			SetRand((ULONG)t);
		}
		else
		{
			printf("Unexpected param! Try use \"-help\"\n");
			return 0;
		}
	}

	if (range == 0)
	{
		range = 16;
	}
	if (seed == 0)
	{
		time(&t);
		SetRand((ULONG)t);
	}

	printf("Lab 3. Pseudo-random sequence generators\n\nSeed = %ld, Range = %d\n", I, range);
	printf("Getting rand value . . .\n");

	for (ULONG i = 0; i < range; i++)
	{
		printf("%d.\t%ld\n", i, GetRandValue());
	}
	return 0;
}
