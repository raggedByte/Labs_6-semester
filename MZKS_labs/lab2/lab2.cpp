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

typedef unsigned int Block;

int key[] = { 12, 13, 30, 5, 27, 6, 11, 25, 3, 21, 22, 2, 23, 0, 8, 4, 18, 19, 10, 1, 14, 29, 9, 28, 20, 17, 26, 31, 7, 16, 15, 24 };
const unsigned int SIZE_BLOCK = (sizeof(key) / sizeof(int));

int getBit(Block block, int numBit)
{
	return ((block & (1 << numBit)) != 0);
}

int setBit(Block block, int numBit)
{
	return (block | (1 << numBit));
}

int unsetBit(Block block, int numBit)
{
	return (block & ~(1 << numBit));
}

void printBlock(Block block, bool debug = false)
{
	if (debug)
	{
		printf("NumBit\t");
		for (int i = SIZE_BLOCK; i > 0; i--)
		{
			printf("%3d", i);
		}
		printf("\n");
	}
	printf("Block\t");

	for (int i = SIZE_BLOCK - 1; i >= 0; i--)
	{
		printf("%3d", getBit(block, i));
	}
	printf("\n");
}

void printKey()
{
	printf("Key\t");
	for (int i = 0; i < SIZE_BLOCK; i++)
	{
		printf("%3d", key[i]);
	}
	printf("\n");
}

int cryptBlock(Block block)
{
	Block tempBlock = 0;
	for (int i = 0; i < SIZE_BLOCK; i++)
	{
		int tempBit = getBit(block, key[i]);
		if (tempBit == 1)
			tempBlock = setBit(tempBlock, i);
	}
	return tempBlock;
}

int decryptBlock(Block block)
{
	Block tempBlock = 0;
	for (int i = 0; i < SIZE_BLOCK; i++)
	{
		int tempBit = getBit(block, i);
		if (tempBit == 1)
			tempBlock = setBit(tempBlock, key[i]);
	}
	return tempBlock;
}

int main()
{
	Block block = 0xA6F5;
	printKey();
	printf("\n");
	printBlock(block, true);
	block = cryptBlock(block);
	printBlock(block);
	block = decryptBlock(block);
	printBlock(block);
}