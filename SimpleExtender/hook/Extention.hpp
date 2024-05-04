#pragma once
#include "trampoline/Trampoline.hpp"
#include <Windows.h>

class Extention
{
public:
	static void ExtendFibo();

private:
	static uint64_t __stdcall Handler(Registers& registers);

private:
	static HMODULE m_Module;
	static Trampoline m_Trampoline;
};