#include <Windows.h>

#include "explorer/ExecutableExplorer.hpp"

#include "pseudoc/PCConverter.hpp"
#include "pseudoc/PCVisualizer.hpp"

#include "utility/LowTrustString.hpp"
#include "utility/CString.hpp"

#include "explorer/StringExplorer.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return true;
}

int main()
{
	ExecutableExplorer explorer = ExecutableExplorer(L"Dismantler.exe");

	//lowTrust.GetStringLength(L"ここで虚がいた", length);


	/*ExecutableExplorer explorer = ExecutableExplorer(L"kernel32.dll");

	delete explorer.GetExecutableFunctions();*/
	
	/*PCConverter converter = PCConverter(explorer.GetBuffer());
	PCVisualizer visualizer = PCVisualizer(explorer.GetBuffer());

	PCBlob blob = converter.Convert(0x14001B9F0);

	visualizer.ToConsole(blob);*/

	/*delete explorer.GetExecutableFunctions();

	delete explorer.GetHeaders();
	delete explorer.GetExecutableView();*/

	delete explorer.GetExecutableStrings();
	delete explorer.GetExecutableFunctions();

	delete explorer.GetPCFunction(0x140014F70);
}