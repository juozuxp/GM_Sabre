#pragma once
#include <Disassembler.hpp>
#include "pe/PEBuffer.hpp"
#include "PCBlob.hpp"

class PCConverter
{
private:
	struct Register
	{
		uint32_t m_VariableHigh = 0;
		uint32_t m_VariableIndex = 0;
	};

	struct State
	{
		uint32_t m_VariableIndex = 1;

		const void* m_Base;
		const void* m_RefBase;

		Register m_General[16];
	};

public:
	void Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const;

private:
	bool ReadOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand, bool create) const;

private:
	Disassembler m_Disassembler;
};