#pragma once
#include <Disassembler.hpp>
#include <unordered_map>

#include "pe/PEBuffer.hpp"
#include "PCBlob.hpp"

class PCConverter
{
private:
	struct RegSpace
	{
		uint64_t m_Value;

		uint32_t m_VariableHigh;
		uint32_t m_VariableIndex;
	};

	struct MemSpace
	{
		uint64_t m_Value;

		uint32_t m_VariableIndex;
	};

	struct State
	{
		const void* m_Cursor;

		size_t m_ImageSize;

		uintptr_t m_ImageBase;
		uintptr_t m_CursorBase;

		RegSpace m_General[16];

		std::unordered_map<uintptr_t, MemSpace> m_Memory;
	};

public:
	void Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const;

private:
	bool ReadOperand(State& state, PCBlob& blob, const ILOperand& asmOperand, PCOperand& pcOperand, bool create) const;

	bool ExecWriteOperand(const ILOperand& operand, State& state, uint64_t value) const;
	bool ExecReadOperand(const ILOperand& operand, const State& state, uint64_t& value) const;
	bool ExecLoadOperand(const ILOperand& operand, const State& state, uint64_t& value) const;

private:
	Disassembler m_Disassembler;
};