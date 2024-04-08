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
ManagedString* ExecutableExplorer_GetPCFunction(const ExecutableExplorer* instance, uintptr_t function)
{
	return instance->GetPCFunction(function);
}