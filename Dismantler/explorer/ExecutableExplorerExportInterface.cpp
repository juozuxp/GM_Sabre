#include "ExecutableExplorer.hpp"

extern "C"
__declspec(dllexport)
ExecutableExplorer* ExecutableExplorer_Init(const wchar_t* path)
{
	return new ExecutableExplorer(path);
}

extern "C"
__declspec(dllexport)
PEHeaders* ExecutableExplorer_GetHeaders(ExecutableExplorer* instance)
{
	return instance->GetHeaders();
}