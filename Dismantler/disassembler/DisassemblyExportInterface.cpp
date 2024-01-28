#include "DisassemblyInterface.hpp"

extern "C"
__declspec(dllexport)
DisassemblyInterface* Disassembler_Init()
{
	return new DisassemblyInterface();
}

extern "C"
__declspec(dllexport)
ManagedGenericArray<Visualizer::Visual>* Disassembler_Disassemble(DisassemblyInterface* instance, void* code, uint32_t size, void* reference)
{
	return instance->Disassemble(code, size, reference);
}