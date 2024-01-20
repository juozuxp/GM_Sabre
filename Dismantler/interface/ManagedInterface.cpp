#include "ManagedInterface.hpp"

extern "C"
__declspec(dllexport)
void ManagedArray_Free(void* array)
{
	delete[] array;
}

extern "C"
__declspec(dllexport)
void Test(ManagedInterface::ManagedArray<ILInstruction>* array)
{
	Disassembler dissasembler;

	uint8_t code[] = { 0x00, 0x00, 0x00, 0x00 };

	*array = ManagedInterface::ManagedArray<ILInstruction>(dissasembler.Disassemble(code, sizeof(code)));
}