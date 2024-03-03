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
ManagedArray<FunctionExplorer::Function>* ExecutableExplorer_GetExecutableFunctions(ExecutableExplorer* instance)
{
	return instance->GetExecutableFunctions();
}