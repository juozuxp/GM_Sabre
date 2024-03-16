#include <Windows.h>

#include "explorer/ExecutableExplorer.hpp"

#include "pseudoc/PCConverter.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return true;
}

int main()
{
	ExecutableExplorer explorer = ExecutableExplorer(L"dxgi.dll");
	PCConverter converter;

	delete explorer.GetExecutableFunctions();

	delete explorer.GetHeaders();
	delete explorer.GetExecutableView();
}