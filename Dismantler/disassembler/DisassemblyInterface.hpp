#pragma once
#include "managed/ManagedObject.hpp"
#include "managed/ManagedGenericArray.hpp"

#include <Disassembler.hpp>
#include <Visualizer.hpp>

class DisassemblyInterface : public ManagedObject
{
public:
	DisassemblyInterface() = default;

public:
	ManagedGenericArray<Visualizer::Visual>* Disassemble(void* code, uint32_t size, void* reference);

private:
	Visualizer m_Visualizer;
	Disassembler m_Disassembler;
};