#include "Extention.hpp"
#include <stdio.h>
#include <utility>

Trampoline Extention::m_Trampoline = Trampoline(Handler);
HMODULE Extention::m_Module = GetModuleHandleA(nullptr);

uint64_t __stdcall Extention::Handler(Registers& registers)
{
	char number[256];

	printf("Recursive approaches tend to be inferior to iterative ones, so I'll show you how to do fibo iteratively ;)\n");
	printf("Just for the sake of proof specify a number that should be added to the result: ");
	fgets(number, sizeof(number), stdin);

	int adder = atoi(number);
	if (registers.m_RCX == 0)
	{
		registers.m_RAX = adder;
		return reinterpret_cast<uintptr_t>(m_Module) + 0x1199E;
	}

	int32_t trail = 0;
	int32_t result = 1;
	for (uint32_t i = 2; i <= registers.m_RCX; i++)
	{
		int32_t temp = result;

		result += trail;
		trail = temp;
	}

	registers.m_RAX = result + adder;
	return reinterpret_cast<uintptr_t>(m_Module) + 0x1199E;
}

void Extention::ExtendFibo()
{
	m_Trampoline.Hook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_Module) + 0x11999));
}