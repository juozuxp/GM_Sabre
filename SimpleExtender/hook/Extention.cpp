#include "Extention.hpp"
#include <stdio.h>
#include <utility>

Trampoline Extention::m_Trampoline = Trampoline(Handler);
HMODULE Extention::m_Module = GetModuleHandleA(nullptr);

uint64_t __stdcall Extention::Handler(Registers& registers)
{
	printf("Recursive approaches tend to be inferior to iterative ones, so I'll show you how to do fibo iteratively ;)\n");

	if (registers.m_RCX == 0)
	{
		return reinterpret_cast<uintptr_t>(m_Module) + 0x1199E;
	}

	int32_t number = 0;
	int32_t result = 1;
	for (uint32_t i = 2; i < registers.m_RCX; i++)
	{
		int32_t temp = result;

		result += number;
		number = result;
	}

	registers.m_RAX = result;
	return reinterpret_cast<uintptr_t>(m_Module) + 0x1199E;
}

void Extention::ExtendFibo()
{
	m_Trampoline.Hook(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_Module) + 0x11999));
}