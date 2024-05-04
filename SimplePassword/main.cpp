#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Windows.h>

int main()
{
	char* password = new char[128];

	printf("Enter first password: ");
	fgets(password, 128, stdin);

	uint32_t length = strlen(password);
	if (length > 0 && password[length - 1] == '\n')
	{
		password[length - 1] = '\0';
	}

	if (strcmp(password, "maybe just a simple input #1") != 0)
	{
		printf("Invalid password\n");
		system("pause");
		return 1;
	}

	printf("Enter second password: ");
	fgets(password, 128, stdin);

	length = strlen(password);
	if (length > 0 && password[length - 1] == '\n')
	{
		password[length - 1] = '\0';
	}

	static uint8_t secondPassword[] = { 0xff, 0x16, 0x1f, 0x75, 0x28, 0x56, 0x51, 0x62, 0xdf, 0x8d, 0xb2, 0xa1, 0xcb, 0xc6, 0xfc, 0xf0, 0x58, 0x75, 0x06, 0x12 };
	char* secondBuffer = new char[20];

	*reinterpret_cast<uint64_t*>(secondBuffer + 0x0) = *reinterpret_cast<const uint64_t*>(secondPassword + 0x0) ^ 0x1122334455667788;
	*reinterpret_cast<uint64_t*>(secondBuffer + 0x8) = *reinterpret_cast<const uint64_t*>(secondPassword + 0x8) ^ 0x8899AABBCCDDEEFF;
	*reinterpret_cast<uint32_t*>(secondBuffer + 0x10) = *reinterpret_cast<const uint32_t*>(secondPassword + 0x10) ^ 0x12345678;

	if (strcmp(password, secondBuffer) != 0)
	{
		delete[] secondBuffer;

		printf("Invalid password\n");
		system("pause");
		return 1;
	}

	delete[] secondBuffer;

	printf("Enter third password: ");
	fgets(password, 128, stdin);

	length = strlen(password);
	if (length > 0 && password[length - 1] == '\n')
	{
		password[length - 1] = '\0';
	}

	static uint8_t thirdPassword[] = { 0x2B, 0x40, 0x3B, 0xD9, 0x77, 0x54, 0x90, 0xBE, 0x99, 0xE4, 0x94, 0xA4, 0x63, 0xDF, 0x1D, 0x40, 0xFA, 0x94, 0x89, 0x6F, 0xAE, 0x64, 0xFF, 0xD9, 0xC2, 0x83, 0x8C, 0xC5, 0x35, 0xC6, 0xD3, 0xB4, 0xD2, 0xCE, 0xD4, 0xCA, 0x71, 0x9F, 0x8A, 0x7E, 0xFE, 0xE3, 0xDE, 0xFE };
	char* thirdBuffer = new char[44];

	*reinterpret_cast<uint64_t*>(thirdBuffer + 0x0) = *reinterpret_cast<const uint64_t*>(thirdPassword + 0x0) + 0xBADC0DE989343245;
	*reinterpret_cast<uint64_t*>(thirdBuffer + 0x8) = *reinterpret_cast<const uint64_t*>(thirdPassword + 0x8) - 0xDEADBEEF31337879;
	*reinterpret_cast<uint64_t*>(thirdBuffer + 0x10) = *reinterpret_cast<const uint64_t*>(thirdPassword + 0x10) ^ 0xBADF00DC00FEE789;
	*reinterpret_cast<uint64_t*>(thirdBuffer + 0x18) = *reinterpret_cast<const uint64_t*>(thirdPassword + 0x18) - 0x4561561551651561;
	*reinterpret_cast<uint64_t*>(thirdBuffer + 0x20) = *reinterpret_cast<const uint64_t*>(thirdPassword + 0x20) - 0x5561655165616565;
	*reinterpret_cast<uint32_t*>(thirdBuffer + 0x28) = *reinterpret_cast<const uint32_t*>(thirdPassword + 0x28) ^ 0xFEEDC0DE;

	if (strcmp(password, thirdBuffer) != 0)
	{
		delete[] thirdBuffer;

		printf("Invalid password\n");
		system("pause");
		return 1;
	}

	delete[] thirdBuffer;

	printf("Logged in\n");
	system("pause");
}