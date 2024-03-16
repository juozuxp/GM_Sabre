#pragma once
#include <Disassembler.hpp>
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
	void Convert(const void* base, const void* function, PCBlob& blob) const;

private:
	bool CreateOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const;
	bool ReadOperand(const State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const;

private:
	Disassembler m_Disassembler;
};