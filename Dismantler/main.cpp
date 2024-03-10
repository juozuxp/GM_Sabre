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
	ExecutableExplorer explorer = ExecutableExplorer(L"dxgi.dll");

	delete explorer.GetExecutableFunctions();

	delete explorer.GetHeaders();
	delete explorer.GetExecutableView();
}