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
	ULONG  range = 0, interval = 0;
	BOOL generate = FALSE, research = FALSE;

	if (argc < 2)
	{
		printf("Expected params! Use -help \n\n");
		return 0;
	}

	for (INT i = 1; i < argc; i++)
	{
		if (strcmp("-help", argv[i]) == 0)
		{
			printf("Help:\n\
\t-help - get help about params\n\
\t-generate <range> - generate random values\n\
\t-research <interval> - research generated values in adjusted intervals\n");
			return 0;
		}
		else if (strcmp("-generate", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			range = atoi(argv[i]);
			generate = TRUE;
		}
		else if (strcmp("-research", argv[i]) == 0)
		{
			if (++i >= argc)
			{
				printf("Expected value after param! Try use \"-help\n");
				return 0;
			}
			interval = atoi(argv[i]);
			research = TRUE;
		}
		else
		{
			printf("Unexpected param! Try use \"-help\"\n");
			return 0;
		}
	}

	time(&t);
	SetRand((ULONG)t);

	printf("Lab 3. Pseudo-random sequence generators\n\n");

	if (generate)
	{
		printf("Generating random values. Seed = %d, range = %d\n", I, range);
		for (ULONG i = 0; i < range; i++)
		{
			printf("%d.\t%ld\n", i, GetRandValue());
		}
		printf("\n");
	}

	if (research)
	{
		PULONG intervals, result;

		printf("Research random values. Interval = %d \n", interval);
		__try
		{
			intervals = (PULONG)malloc(sizeof(ULONG) * interval);
			result = (PULONG)malloc(sizeof(ULONG) * interval);
			if (intervals == 0 || result == 0)
			{
				printf("Error allocation memory!\n");
				return -1;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			printf("Error allocation memory!\n");
			return -1;
		}

		memset(intervals, 0, sizeof(PULONG) * interval);
		memset(result, 0, sizeof(PULONG) * interval);

		for (UINT i = 1; i < interval + 1; i++)
			intervals[i - 1] = MAXLONG / interval * i;

		__try
		{
			for (ULONG k = 0; k < MAXLONG; k++)
			{
				ULONG randValue = GetRandValue();
				for (UINT i = 0; i < interval; i++)
				{
					if (randValue <= intervals[i])
					{
						result[i]++;
						break;
					}
				}
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			printf("Error while research values!\n");
			return -1;
		}

		for (UINT i = 0; i < interval; i++)
		{
			printf("%d.\t%d\n", intervals[i], result[i]);
		}
	}
	return 0;
}
