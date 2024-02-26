#include <Windows.h>

#include "explorer/ExecutableExplorer.hpp"

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
	ExecutableExplorer explorer = ExecutableExplorer(L"actxprxy.dll");

	delete explorer.GetHeaders();
	delete explorer.GetExecutableView();
}