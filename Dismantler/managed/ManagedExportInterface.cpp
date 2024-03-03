#include <Disassembler.hpp>
#include <Visualizer.hpp>

#include "ManagedObject.hpp"
#include "ManagedArray.hpp"
#include "ManagedString.hpp"

#include <Windows.h>

extern "C"
__declspec(dllexport)
void Managed_Destroy(ManagedObject* handle)
{
	delete handle;
}