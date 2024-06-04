#include <stdio.h>
#include <Windows.h>
#include <stdint.h>

using CreateInterface = bool(*)(int32_t style, int32_t width, int32_t height);

int main()
{
	HMODULE module = LoadLibraryA("SimpleErrorable.dll");
	if (module == nullptr)
	{
		printf("No module found\n");
		system("pause");

		return 1;
	}

	CreateInterface createInstance = reinterpret_cast<CreateInterface>(GetProcAddress(module, "CreateInterface"));
	if (createInstance(0x62, 500, 406))
	{
		printf("success!!!\n");
		system("pause");

		return 0;
	}

	printf("failed :(\n");
	system("pause");

	return 1;
}