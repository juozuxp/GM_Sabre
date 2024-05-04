#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t Fibo(int32_t i)
{
	if (i <= 1)
	{
		return i;
	}

	return Fibo(i - 1) + Fibo(i - 2);
}

int main()
{
	char input[256];

	printf("Enter fibo iteration number: ");
	fgets(input, sizeof(input), stdin);

	int32_t n = atoi(input);

	printf("The fibo number of the %i iteration is equal to %i\n", n, Fibo(n));
	system("pause");
}