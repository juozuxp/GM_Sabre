#include "ExecutableExplorer.hpp"

extern "C"
__declspec(dllexport)
ExecutableExplorer* ExecutableExplorer_Init(const wchar_t* path)
{
	return new ExecutableExplorer(path);
}

extern "C"
__declspec(dllexport)
PEHeaders* ExecutableExplorer_GetHeaders(const ExecutableExplorer* instance)
{
	return instance->GetHeaders();
}

extern "C"
__declspec(dllexport)
ExecutableView* ExecutableExplorer_GetExecutableView(const ExecutableExplorer* instance)
{
	return instance->GetExecutableView();
}

extern "C"
__declspec(dllexport)
ManagedArray<ExecutableExplorer::ManagedFunction>* ExecutableExplorer_GetExecutableFunctions(ExecutableExplorer* instance)
{
	return instance->GetExecutableFunctions();
}

extern "C"
__declspec(dllexport)
ManagedString* ExecutableExplorer_GetPCFunction(ExecutableExplorer* instance, uintptr_t function)
{
	return instance->GetPCFunction(function);
}

extern "C"
__declspec(dllexport)
ManagedArray<ExecutableExplorer::ManagedStringEntry>* ExecutableExplorer_GetExecutableStrings(ExecutableExplorer* instance)
{
	return instance->GetExecutableStrings();
}

extern "C"
__declspec(dllexport)
ManagedArray<XRefExplorer::Entry>* ExecutableExplorer_GetAddressXRefs(const ExecutableExplorer* instance, uintptr_t address)
{
	return instance->GetAddressXRefs(address);
}