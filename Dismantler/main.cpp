#include <Disassembler.hpp>
#include <Visualizer.hpp>
#include <Windows.h>

#include "disassembler/DisassemblyInterface.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_THREAD_ATTACH:
	{

	} break;
	}

	return true;
}

int main()
{
}