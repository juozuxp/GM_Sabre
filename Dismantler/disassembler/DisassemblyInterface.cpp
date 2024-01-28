#include "DisassemblyInterface.hpp"

#include <Disassembler.hpp>

ManagedGenericArray<Visualizer::Visual>* DisassemblyInterface::Disassemble(void* code, uint32_t size, void* reference)
{
	std::vector<ILInstruction> instructions = m_Disassembler.Disassemble(code, size);
	std::vector<Visualizer::Visual> visuals = m_Visualizer.ToVisuals(instructions, reference);

	return new ManagedGenericArray<Visualizer::Visual>(visuals);
}