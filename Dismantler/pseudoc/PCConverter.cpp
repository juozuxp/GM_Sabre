#include "PCConverter.hpp"
#include <vector>

#define CHUNK_SIZE 0x200 

void PCConverter::Convert(const void* base, const void* function, PCBlob& blob) const
{
	std::vector<ILInstruction> instructions;

	State state;

	state.m_RefBase = base;
	state.m_Base = function;
	while (true)
	{
		uint32_t chunk = (1 << 12) - (reinterpret_cast<uintptr_t>(function) & ((1 << 12) - 1));
		if (chunk > CHUNK_SIZE ||
			chunk == 0)
		{
			chunk = CHUNK_SIZE;
		}

		instructions.clear();

		m_Disassembler.Disassemble(function, chunk, instructions);
		for (const ILInstruction& ins : instructions)
		{
			state.m_Base = reinterpret_cast<const uint8_t*>(state.m_Base) + ins.m_Size;
			state.m_RefBase = reinterpret_cast<const uint8_t*>(state.m_RefBase) + ins.m_Size;

			switch (ins.m_Type)
			{
			case InsType_mov:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				PCInstruction pseudo;

				if (!ReadOperand(state, rhs, pseudo.m_Equal.m_Rhs))
				{
					break;
				}

				if (!CreateOperand(state, lhs, pseudo.m_Equal.m_Lhs))
				{
					break;
				}

				blob.m_Instructions.push_back(pseudo);
			} break;
			case InsType_ret:
			case InsType_int3:
			case InsType_ud0:
			case InsType_ud1:
			case InsType_ud2:
			case InsType_invalid:
			{
				return;
			} break;
			}
		}
	}
}
 
bool PCConverter::ReadOperand(const State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (asmOperand.m_Type)
	{
	case ILOperandType_Memory:
	{
		pcOperand.m_Type = PCOperand::Type::Dereference;

		uint32_t base = 0;
		if (asmOperand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			base = state.m_General[asmOperand.m_Memory.m_Base].m_VariableIndex;
			if (base == 0)
			{
				return false;
			}
		}

		uint32_t index = 0;
		if (asmOperand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			index = state.m_General[asmOperand.m_Memory.m_Index].m_VariableIndex;
			if (index == 0)
			{
				return false;
			}
		}

		pcOperand.m_Dereference.m_BaseVariable = base;
		pcOperand.m_Dereference.m_IndexVariable = index;
		pcOperand.m_Dereference.m_Scale = asmOperand.m_Scale;
		pcOperand.m_Dereference.m_Offset = asmOperand.m_Memory.m_Offset;
		pcOperand.m_Dereference.m_Multiplier = multiplier[asmOperand.m_Memory.m_Scale];
	} break;
	case ILOperandType_Register:
	{
		pcOperand.m_Type = PCOperand::Type::Variable;

		uint32_t base = 0;
		if (asmOperand.m_Register.m_BaseHigh)
		{
			base = state.m_General[asmOperand.m_Register.m_Base].m_VariableHigh;
			if (base == 0)
			{
				return false;
			}
		}
		else
		{
			base = state.m_General[asmOperand.m_Register.m_Base].m_VariableIndex;
			if (base == 0)
			{
				return false;
			}
		}

		pcOperand.m_Variable.m_Index = base;
	} break;
	case ILOperandType_Value:
	{

	} break;
	case ILOperandType_ValueRelative:
	{

	} break;
	case ILOperandType_MemoryRelative:
	{

	} break;
	case ILOperandType_MemoryAbsolute:
	{

	} break;
	}

	return true;
}

bool PCConverter::CreateOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (asmOperand.m_Type)
	{
	case ILOperandType_Memory:
	case ILOperandType_Value:
	case ILOperandType_ValueRelative:
	case ILOperandType_MemoryRelative:
	case ILOperandType_MemoryAbsolute:
	{
		return ReadOperand(state, asmOperand, pcOperand);
	} break;
	case ILOperandType_Register:
	{
		pcOperand.m_Type = PCOperand::Type::Variable;

		uint32_t base = 0;
		if (asmOperand.m_Register.m_BaseHigh)
		{
			base = state.m_General[asmOperand.m_Register.m_Base].m_VariableHigh;
			if (base == 0)
			{
				base = state.m_VariableIndex++;
				state.m_General[asmOperand.m_Register.m_Base].m_VariableHigh = base;
			}
		}
		else
		{
			base = state.m_General[asmOperand.m_Register.m_Base].m_VariableIndex;
			if (base == 0)
			{
				base = state.m_VariableIndex++;
				state.m_General[asmOperand.m_Register.m_Base].m_VariableIndex = base;
			}
		}

		pcOperand.m_Variable.m_Index = base;
	} break;
	}

	return true;
}