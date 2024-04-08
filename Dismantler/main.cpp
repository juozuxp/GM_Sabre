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
	ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

	delete explorer.GetExecutableFunctions();
	
	/*PCConverter converter = PCConverter(explorer.GetBuffer());
	PCVisualizer visualizer = PCVisualizer(explorer.GetBuffer());

	PCBlob blob = converter.Convert(0x14001B9F0);

	visualizer.ToConsole(blob);*/

	/*delete explorer.GetExecutableFunctions();

	delete explorer.GetHeaders();
	delete explorer.GetExecutableView();*/
}