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

#include <cstdio>
#include <time.h>
#include <string.h>
#include "stdlib.h"

unsigned long int I = 0;
const unsigned long int a = 16807;
const unsigned long int c = 4;
const unsigned long int m = 2147483648 - 1;

void SetRand(unsigned long int seed)
{
	I = seed;
}

unsigned long int GetRandValue()
{
	I = (a * I + c) % m;
	return I;
}

int main(int argc, char* argv[])
{
	time_t t;
	unsigned long int range = 0, seed = 0;
	bool bSeed = false;
	if (argc == 1)
	{
		printf("Range and seed was set to default . . \n\n");
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp("-help", argv[i]) == 0)
			{
				printf("Help:\n\t-help - get help about params\n\t-range <value> - set range printable rand values \
\n\t-seed <value> - set new seed for rand function\n");
				return 0;
			}
			else if (strcmp("-range", argv[i]) == 0)
			{
				if (i + 1 >= argc)
				{
					printf("Expected value after param! Try use \"-help\n");
					return 0;
				}
				i++;
				range = atoi(argv[i]);
			}
			else if (strcmp("-seed", argv[i]) == 0)
			{
				if (i + 1 >= argc)
				{
					printf("Expected value after param! Try use \"-help\n");
					return 0;
				}
				i++;
				seed = atoi(argv[i]);
				bSeed = true;
			}
			else
			{
				printf("Unexpected param! Try use \"-help\"\n");
				return 0;
			}
		}
	}

	if (bSeed == false)
	{
		time(&t);
		SetRand((unsigned long)t);
	}
	else
	{
		SetRand(seed);
	}
	if (range == 0)
	{
		range = 16;
	}
	printf("Lab 3. Pseudo-random sequence generators\n\nSeed = %ld, Range = %d\n", I, range);
	printf("Getting rand value . . .\n");
	for (int i = 0; i < range; i++)
	{
		printf("%d.\t%ld\n", i, GetRandValue());
	}
	return 0;
}
