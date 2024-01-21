#include "ManagedInterface.hpp"

extern "C"
__declspec(dllexport)
void Dismantler_Init(Disassembler** handle)
{
	*handle = new Disassembler();
}

extern "C"
__declspec(dllexport)
void Dismantler_Destroy(Disassembler* handle)
{
	delete handle;
}

extern "C"
__declspec(dllexport)
void Dismantler_Dissasemble(Disassembler* handle, void* address, uint64_t size, ManagedInterface::ManagedArray<ILInstruction>* array)
{
	*array = ManagedInterface::ManagedArray<ILInstruction>(handle->Disassemble(address, size));
}

extern "C"
__declspec(dllexport)
void ManagedArray_Destroy(void* array)
{
	delete[] array;
}