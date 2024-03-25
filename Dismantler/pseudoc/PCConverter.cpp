#include "PCConverter.hpp"
#include <vector>
#include <format>

#define CHUNK_SIZE 0x200 

void PCConverter::Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const
{
	constexpr uint64_t masks[] = { 0, (1ull << 8) - 1, (1ull << 16) - 1, (1ull << 32) - 1, ~0 };

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

	state.m_Cursor = reinterpret_cast<const void*>(reinterpret_cast<uintptr_t>(dos) + function - optional->ImageBase);

	state.m_ImageBase = optional->ImageBase;
	state.m_ImageSize = optional->SizeOfImage;
	state.m_CursorBase = reinterpret_cast<uintptr_t>(dos);

	state.m_General[REG_RSP].m_Value = state.m_ImageBase + (1ull << 40) - 8;
	while (true)
	{
		uint32_t chunk = (1 << 12) - (reinterpret_cast<uintptr_t>(state.m_Cursor) & ((1 << 12) - 1));
		if (chunk > CHUNK_SIZE ||
			chunk == 0)
		{
			chunk = CHUNK_SIZE;
		}

		instructions.clear();

		m_Disassembler.Disassemble(state.m_Cursor, chunk, instructions);
		for (const ILInstruction& ins : instructions)
		{
			state.m_Cursor = reinterpret_cast<const uint8_t*>(state.m_Cursor) + ins.m_Size;

			switch (ins.m_Type)
			{
			case InsType_push:
			{
				const ILOperand& lhs = ins.m_Operands[0];

				uint64_t value;
				if (ExecReadOperand(lhs, state, value))
				{
					state.m_General[REG_RSP].m_Value -= 8;
					state.m_Memory[state.m_General[REG_RSP].m_Value].m_Value = value & masks[lhs.m_Scale];
				}
			} break;
			case InsType_sub:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t lhsValue;
				uint64_t rhsValue;
				if (ExecReadOperand(lhs, state, lhsValue) &&
					ExecReadOperand(rhs, state, rhsValue))
				{
					ExecWriteOperand(lhs, state, lhsValue - rhsValue);
				}

				PCInstruction pseudo = {};

				pseudo.m_Type = PCInstruction::Type::Subtract;

				if (!ReadOperand(state, blob, rhs, pseudo.m_Double.m_Rhs, false))
				{
					break;
				}

				if (!ReadOperand(state, blob, lhs, pseudo.m_Double.m_Lhs, false))
				{
					break;
				}

				blob.m_Instructions.push_back(pseudo);
			} break;
			case InsType_lea:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t value;
				if (ExecLoadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

			} break;
			case InsType_mov:
			{
				const ILOperand& lhs = ins.m_Operands[0];
				const ILOperand& rhs = ins.m_Operands[1];

				uint64_t value;
				if (ExecReadOperand(rhs, state, value))
				{
					ExecWriteOperand(lhs, state, value);
				}

				PCInstruction pseudo = {};

				pseudo.m_Type = PCInstruction::Type::Assign;

				if (!ReadOperand(state, blob, rhs, pseudo.m_Double.m_Rhs, false))
				{
					break;
				}

				if (!ReadOperand(state, blob, lhs, pseudo.m_Double.m_Lhs, true))
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
 
bool PCConverter::ReadOperand(State& state, PCBlob& blob, const ILOperand& asmOperand, PCOperand& pcOperand, bool create) const
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
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Base];
			if (reg.m_VariableIndex != 0)
			{
				return false;
			}

			base = reg.m_VariableIndex;
		}

		uint32_t index = 0;
		if (asmOperand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			RegSpace& reg = state.m_General[asmOperand.m_Memory.m_Index];
			if (reg.m_VariableIndex != 0)
			{
				return false;
			}

			index = reg.m_VariableIndex;
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
			RegSpace& reg = state.m_General[asmOperand.m_Register.m_Base];
			if (reg.m_VariableHigh != 0)
			{
				if (!create)
				{
					return false;
				}
				
				base = blob.m_Variables.size() + 1;

				PCVariable variable;

				variable.m_Size = asmOperand.m_Scale;
				variable.m_Name = "v" + std::to_string(base);
				variable.m_Type = PCVariable::Type::Local;

				blob.m_Variables.push_back(variable);
				state.m_General[asmOperand.m_Register.m_Base].m_VariableHigh = base;
			}
			else
			{
				base = reg.m_VariableHigh;
			}
		}
		else
		{
			RegSpace& reg = state.m_General[asmOperand.m_Register.m_Base];
			if (base == 0)
			{
				if (!create)
				{
					return false;
				}

				base = blob.m_Variables.size() + 1;

				PCVariable variable;

				variable.m_Size = asmOperand.m_Scale;
				variable.m_Name = "v" + std::to_string(base);
				variable.m_Type = PCVariable::Type::Local;

				blob.m_Variables.push_back(variable);
				state.m_General[asmOperand.m_Register.m_Base].m_VariableIndex = base;
			}
			else
			{
				base = reg.m_VariableIndex;
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
		pcOperand.m_Type = PCOperand::Type::Literal;
		pcOperand.m_Literal = reinterpret_cast<uintptr_t>(state.m_Cursor) + asmOperand.m_Relative.m_Value - state.m_CursorBase + state.m_ImageBase;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uintptr_t>(state.m_Cursor) + asmOperand.m_Relative.m_Value - state.m_CursorBase + state.m_ImageBase;

		pcOperand.m_Type = PCOperand::Type::Dereference;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = blob.m_Variables.size() + 1;

			PCVariable variable;

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = PCVariable::Type::Static;
			variable.m_Name = "sv" + std::format("{:X}", address);

			blob.m_Variables.push_back(variable);
		}

		pcOperand.m_Dereference.m_BaseVariable = space.m_VariableIndex;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = asmOperand.m_MemoryValue.m_Value;

		pcOperand.m_Type = PCOperand::Type::Dereference;

		MemSpace& space = state.m_Memory[address];
		if (space.m_VariableIndex == 0)
		{
			space.m_VariableIndex = blob.m_Variables.size() + 1;

			PCVariable variable;

			variable.m_Size = asmOperand.m_Scale;
			variable.m_Type = PCVariable::Type::Static;
			variable.m_Name = "sv" + std::format("{:X}", address);

			blob.m_Variables.push_back(variable);
		}

		pcOperand.m_Dereference.m_BaseVariable = space.m_VariableIndex;
	} break;
	}

	return true;
}

bool PCConverter::ExecWriteOperand(const ILOperand& operand, State& state, uint64_t value) const
{
	constexpr uint64_t masks[] = { 0, (1ull << 8) - 1, (1ull << 16) - 1, (1ull << 32) - 1, ~0 };
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (operand.m_Type)
	{
	case ILOperandType_Register:
	{
		if (operand.m_Register.m_Type != Register::general)
		{
			return false;
		}

		if (operand.m_Scale == ILOperandScale_8)
		{
			if (operand.m_Register.m_BaseHigh)
			{
				state.m_General[operand.m_Register.m_Base].m_Value = (state.m_General[operand.m_Register.m_Base].m_Value & ~(((1 << 8) - 1) << 8)) | ((value & ((1 << 8) - 1)) << 8);
			}
			else
			{
				state.m_General[operand.m_Register.m_Base].m_Value = (state.m_General[operand.m_Register.m_Base].m_Value & ~((1 << 8) - 1)) | (value & ((1 << 8) - 1));
			}

			return true;
		}

		if (operand.m_Scale == ILOperandScale_16)
		{
			state.m_General[operand.m_Register.m_Base].m_Value = (state.m_General[operand.m_Register.m_Base].m_Value & ~((1 << 16) - 1)) | (value & ((1 << 16) - 1));
			return true;
		}

		state.m_General[operand.m_Register.m_Base].m_Value = value & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_Memory:
	{
		uint64_t address = 0;
		if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			address = state.m_General[operand.m_Memory.m_Base].m_Value;
		}

		if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			address += state.m_General[operand.m_Memory.m_Index].m_Value * multiplier[operand.m_Memory.m_Scale];
		}

		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				*reinterpret_cast<uint8_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_16:
			{
				*reinterpret_cast<uint16_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_32:
			{
				*reinterpret_cast<uint32_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_64:
			{
				*reinterpret_cast<uint64_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			}
		}
		else
		{
			state.m_Memory[address].m_Value = value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = operand.m_MemoryValue.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				*reinterpret_cast<uint8_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_16:
			{
				*reinterpret_cast<uint16_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_32:
			{
				*reinterpret_cast<uint32_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_64:
			{
				*reinterpret_cast<uint64_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			}
		}
		else
		{
			state.m_Memory[address].m_Value = value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uint64_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				*reinterpret_cast<uint8_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_16:
			{
				*reinterpret_cast<uint16_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_32:
			{
				*reinterpret_cast<uint32_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			case ILOperandScale_64:
			{
				*reinterpret_cast<uint64_t*>(address - state.m_ImageBase + state.m_CursorBase) = value;
			} break;
			}
		}
		else
		{
			state.m_Memory[address].m_Value = value & masks[operand.m_Scale];
		}

		return true;
	} break;
	}

	return false;
}

bool PCConverter::ExecReadOperand(const ILOperand& operand, const State& state, uint64_t& value) const
{
	constexpr uint64_t masks[] = { 0, (1ull << 8) - 1, (1ull << 16) - 1, (1ull << 32) - 1, ~0 };
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (operand.m_Type)
	{
	case ILOperandType_Memory:
	{
		uint64_t address = 0;
		if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			address = state.m_General[operand.m_Memory.m_Base].m_Value;
		}

		if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			address += state.m_General[operand.m_Memory.m_Index].m_Value * multiplier[operand.m_Memory.m_Scale];
		}

		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				value = *reinterpret_cast<const uint8_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_16:
			{
				value = *reinterpret_cast<const uint16_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_32:
			{
				value = *reinterpret_cast<const uint32_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_64:
			{
				value = *reinterpret_cast<const uint64_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			}
		}
		else
		{
			const auto& iterator = state.m_Memory.find(address);
			if (iterator == state.m_Memory.end())
			{
				return false;
			}

			value = iterator->second.m_Value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_Register:
	{
		if (operand.m_Register.m_Type != Register::general)
		{
			return false;
		}

		if (operand.m_Scale == ILOperandScale_8 &&
			operand.m_Register.m_BaseHigh)
		{
			value = (state.m_General[operand.m_Register.m_Base].m_Value >> 8) & ((1 << 8) - 1);
			return true;
		}

		value = state.m_General[operand.m_Register.m_Base].m_Value & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_Value:
	{
		value = operand.m_Value;
		return true;
	} break;
	case ILOperandType_ValueRelative:
	{
		value = (reinterpret_cast<uintptr_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value) & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		uint64_t address = operand.m_MemoryValue.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				value = *reinterpret_cast<const uint8_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_16:
			{
				value = *reinterpret_cast<const uint16_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_32:
			{
				value = *reinterpret_cast<const uint32_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_64:
			{
				value = *reinterpret_cast<const uint64_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			}
		}
		else
		{
			const auto& iterator = state.m_Memory.find(address);
			if (iterator == state.m_Memory.end())
			{
				return false;
			}

			value = iterator->second.m_Value & masks[operand.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		uint64_t address = reinterpret_cast<uint64_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value;
		if (address >= state.m_ImageBase && address < (state.m_ImageBase + state.m_ImageSize))
		{
			switch (operand.m_Scale)
			{
			case ILOperandScale_8:
			{
				value = *reinterpret_cast<const uint8_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_16:
			{
				value = *reinterpret_cast<const uint16_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_32:
			{
				value = *reinterpret_cast<const uint32_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			case ILOperandScale_64:
			{
				value = *reinterpret_cast<const uint64_t*>(address - state.m_ImageBase + state.m_CursorBase);
			} break;
			}
		}
		else
		{
			const auto& iterator = state.m_Memory.find(address);
			if (iterator == state.m_Memory.end())
			{
				return false;
			}

			value = iterator->second.m_Value & masks[operand.m_Scale];
		}

		return true;
	} break;
	}

	return false;
}

bool PCConverter::ExecLoadOperand(const ILOperand& operand, const State& state, uint64_t& value) const
{
	constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

	switch (operand.m_Type)
	{
	case ILOperandType_Memory:
	{
		value = 0;
		if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
		{
			value = state.m_General[operand.m_Memory.m_Base].m_Value;
		}

		if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			value += state.m_General[operand.m_Memory.m_Index].m_Value * multiplier[operand.m_Memory.m_Scale];
		}

		return true;
	} break;
	case ILOperandType_MemoryRelative:
	{
		value = reinterpret_cast<uintptr_t>(state.m_Cursor) - state.m_CursorBase + state.m_ImageBase + operand.m_Relative.m_Value;
		return true;
	} break;
	case ILOperandType_MemoryAbsolute:
	{
		value = operand.m_MemoryValue.m_Value;
		return true;
	} break;
	}

	return false;
}