#include <Windows.h>

#include "explorer/ExecutableExplorer.hpp"

#include "pseudoc/PCConverter.hpp"
#include "pseudoc/PCVisualizer.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return true;
}

int main()
{
	ExecutableExplorer explorer = ExecutableExplorer(L"Injector.exe");
	
	PCConverter converter;
	PCVisualizer visualizer;

	PCBlob blob = converter.Convert(explorer.GetBuffer(), 0x14001A340);

	visualizer.ToConsole(blob);

	/*delete explorer.GetExecutableFunctions();

	delete explorer.GetHeaders();
	delete explorer.GetExecutableView();*/
}