#include <stdio.h>
#include <stdint.h>

extern "C"
__declspec(dllexport)
bool CreateInterface(int32_t style, int32_t width, int32_t height)
{
	if ((style ^ width) != height)
	{
		printf("Error has occured...\n");
		return false;
	}

	return true;
}