#include <cstdio>

int main()
{
	float rX, rY;
	float pX, pY;
	float radius;
	printf("Write coords of round = ");
	if (scanf_s("%f %f", &rX, &rY) != 2)
	{
		printf("Wrote invalid value!");
		return -1;
	}
	printf("Write coords of point = ");
	if (scanf_s("%f %f", &pX, &pY) != 2)
	{
		printf("Wrote invalid value!");
		return -1;
	}
	printf("Write radius of round = ");
	if (scanf_s("%f", &radius) != 1)
	{
		printf("Wrote invalid value!");
		return -1;
	}
	printf("\nXr = %.3f, Yr = %.3f\nXp = %.3f, Yp = %.3f\nRadius = %.3f\n\n", rX, rY, pX, pY, radius);

	if (((pX - rX) * (pX - rX) + (pY - rY) * (pY - rY)) < radius * radius)	//точка принадлежит окружности
	{
		if (pX > rX && pY < rY)
		{
			printf("Point don't belong to round!\n\n");
			return 0;
		}

		printf("Point belong to round!\n\n");
	}
	else if (((pX - rX) * (pX - rX) + (pY - rY) * (pY - rY)) == radius * radius)
	{
		printf("Point lay on round!\n\n");
	}
	else
	{
		printf("Point don't belong to round!\n\n");
	}
}