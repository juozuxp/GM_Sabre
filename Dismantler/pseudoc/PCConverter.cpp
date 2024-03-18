#include "PCConverter.hpp"
#include <vector>

#define CHUNK_SIZE 0x200 

void PCConverter::Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.GetBuffer());
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);
	const IMAGE_FILE_HEADER* file = &nt->FileHeader;

	if (file->Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return;
	}

	const IMAGE_OPTIONAL_HEADER64* optional = &nt->OptionalHeader;

	std::vector<ILInstruction> instructions;

	State state;

	state.m_Base = reinterpret_cast<const uint8_t*>(dos) + (function - optional->ImageBase);
	state.m_RefBase = reinterpret_cast<const void*>(function);
	while (true)
	{
		uint32_t chunk = (1 << 12) - (reinterpret_cast<uintptr_t>(state.m_Base) & ((1 << 12) - 1));
		if (chunk > CHUNK_SIZE ||
			chunk == 0)
		{
			chunk = CHUNK_SIZE;
		}

		instructions.clear();

		m_Disassembler.Disassemble(state.m_Base, chunk, instructions);
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

				pseudo.m_Type = PCInstruction::Type::Assign;

				if (!ReadOperand(state, rhs, pseudo.m_Assign.m_Rhs, false))
				{
					break;
				}

				if (!ReadOperand(state, lhs, pseudo.m_Assign.m_Lhs, true))
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
 
bool PCConverter::ReadOperand(State& state, const ILOperand& asmOperand, PCOperand& pcOperand, bool create) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	pcOperand.m_Scale = asmOperand.m_Scale;
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
				if (!create)
				{
					return false;
				}

				base = state.m_VariableIndex++;
				state.m_General[asmOperand.m_Register.m_Base].m_VariableHigh = base;
			}
		}
		else
		{
			base = state.m_General[asmOperand.m_Register.m_Base].m_VariableIndex;
			if (base == 0)
			{
				if (!create)
				{
					return false;
				}

				base = state.m_VariableIndex++;
				state.m_General[asmOperand.m_Register.m_Base].m_VariableIndex = base;
			}
		}

		pcOperand.m_Variable.m_Index = base;
	} break;
	case ILOperandType_Value:
	{
		pcOperand.m_Type = PCOperand::Type::Literal;
		pcOperand.m_Literal = asmOperand.m_Value;
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