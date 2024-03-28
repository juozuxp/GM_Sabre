#pragma once
#include <Disassembler.hpp>
#include <unordered_map>

#include "pe/PEBuffer.hpp"
#include "PCBlob.hpp"

class PCConverter
{
private:
	enum class SpaceType
	{
		Normal,
		Result,

		Argument0,
		Argument1,
		Argument2,
		Argument3,

		Argument_Start = Argument0,
		Argument_End = Argument3
	};

	enum class BlockType
	{
		None,
		Pointer,
		Instruction
	};

	struct Block
	{
		BlockType m_Type = BlockType::None;
		uint32_t m_Index = 0;

		union
		{
			const void* m_Pointer;
			PCInstruction m_Instruction;
		};
	};

	struct RegSpace
	{
		SpaceType m_Type;

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
		PCBlob* m_Blob;

		const void* m_Cursor;
		const void* m_LastValid;

		size_t m_ImageSize;
		uint64_t m_StackBase;

		uintptr_t m_ImageBase;
		uintptr_t m_CursorBase;

		RegSpace m_General[16];

		std::unordered_map<uintptr_t, MemSpace> m_Memory;
	};

public:
	void Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const;

private:
	bool ClearOperand(State& state, const ILOperand& operand) const;
	bool LoadOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const;
	bool ReadOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const;
	bool WriteOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const;

	bool ExecWriteOperand(const ILOperand& operand, State& state, uint64_t value) const;
	bool ExecReadOperand(const ILOperand& operand, const State& state, uint64_t& value) const;
	bool ExecLoadOperand(const ILOperand& operand, const State& state, uint64_t& value) const;

private:
	Disassembler m_Disassembler;
};