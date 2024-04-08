#include <stdio.h>
#include <stdint.h>

uint32_t Fibo(uint32_t n)
{
	if (n <= 1)
	{
		return n;
	}

	return Fibo(n - 1) + Fibo(n - 2);
}

int main()
{
	printf("fibo 10 = %u", Fibo(10));
}