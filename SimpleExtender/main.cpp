#include <Windows.h>

#include "hook/Extention.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		Extention::ExtendFibo();
	} break;
	}

	return true;
}