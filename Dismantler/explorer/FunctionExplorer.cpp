#include "FunctionExplorer.hpp"

#define CHUNK_SIZE 200

FunctionExplorer::Function::Function(const void* base, uint32_t size) :
	m_Base(base), m_Size(size)
{
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExplorePEFunctions(const PEBuffer& buffer) const
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.GetBuffer());
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dos) + dos->e_lfanew);

	if (nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return std::vector<FunctionExplorer::Function>();
	}

	const IMAGE_OPTIONAL_HEADER64* optional = &nt->OptionalHeader;

	return ExploreFunction(reinterpret_cast<const uint8_t*>(dos) + optional->AddressOfEntryPoint);
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExploreFunction(const void* function) const
{
	std::unordered_set<const void*> explored;
	uint32_t size;

	State state;
	
	explored.insert(function);

	state.m_Stack.push_back(0); // sub rsp, 20
	state.m_Stack.push_back(0); // sub rsp, 20
	state.m_Stack.push_back(0); // sub rsp, 20
	state.m_Stack.push_back(0); // sub rsp, 20
	state.m_Stack.push_back(0); // call function

	std::vector<Function> functions = ExploreBranch(function, function, state, explored, size);

	functions.push_back(Function(function, size));
	return functions;
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExploreBranch(const void* base, const void* branch, State state, std::unordered_set<const void*>& explored, uint32_t& size) const
{
	std::vector<ILInstruction> instructions;
	std::vector<Function> functions;

	size = 0;
	explored.insert(branch);

	uint64_t enterSP = state.m_General[REG_RSP];
	while (true)
	{
		instructions.clear();

		uint32_t chunkSize = ((1 << 12) - reinterpret_cast<intptr_t>(branch) & ((1 << 12) - 1));
		if (CHUNK_SIZE < chunkSize ||
			chunkSize == 0)
		{
			chunkSize = CHUNK_SIZE;
		}

		m_Disassembler.Disassemble(branch, chunkSize, instructions);

		for (const ILInstruction& instruction : instructions)
		{
			branch = reinterpret_cast<const uint8_t*>(branch) + instruction.m_Size;
			size += instruction.m_Size;

			bool breakout = false;
			switch (instruction.m_Type)
			{
			case InsType_mov:
			{
				HandleMov(instruction, state);
			} break;
			case InsType_add:
			{
				HandleAdd(instruction, state);
			} break;
			case InsType_sub:
			{
				HandleSub(instruction, state);
			} break;
			case InsType_jmp:
			{
				const ILOperand& dest = instruction.m_Operands[0];
				if (dest.m_Type != ILOperandType_ValueRelative)
				{
					return functions;
				}

				branch = reinterpret_cast<const uint8_t*>(branch) + dest.m_Relative.m_Value;
				if (enterSP == state.m_General[REG_RSP])
				{
					functions.push_back(Function(base, size));

					base = branch;
					if (explored.contains(base))
					{
						return functions;
					}

					explored.insert(branch);
					size = 0;
				}
				else
				{
					size += dest.m_Relative.m_Value;
				}


				breakout = true;
			} break;
			case InsType_call:
			{
				const ILOperand& dest = instruction.m_Operands[0];
				if (dest.m_Type != ILOperandType_ValueRelative)
				{
					continue;
				}

				const void* function = reinterpret_cast<const uint8_t*>(branch) + dest.m_Relative.m_Value;

				if (!explored.contains(function))
				{
					std::vector<Function> result = ExploreBranch(function, function, state, explored, size);

					functions.insert(functions.end(), result.begin(), result.end());
					functions.push_back(Function(function, size));
				}
			} break;
			case InsType_ret:
			{
				return functions;
			} break;
			}

			if (breakout)
			{
				break;
			}
		}
	}

	return {};
}

void FunctionExplorer::HandleMov(const ILInstruction& instruction, State& state)
{
	const ILOperand& lhs = instruction.m_Operands[0];
	const ILOperand& rhs = instruction.m_Operands[1];

	uint64_t value;
	if (rhs.m_Type == ILOperandType_Register)
	{
		if (rhs.m_Register.m_Type != Register::general)
		{
			return;
		}

		switch (rhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			if (rhs.m_Register.m_BaseHigh)
			{
				value = (state.m_General[rhs.m_Register.m_Base] >> 8) & ((1ull << 8) - 1);
			}
			else
			{
				value = state.m_General[rhs.m_Register.m_Base] & ((1ull << 8) - 1);
			}
		} break;
		case ILOperandScale_16:
		{
			value = state.m_General[rhs.m_Register.m_Base] & ((1ull << 16) - 1);
		} break;
		case ILOperandScale_32:
		{
			value = state.m_General[rhs.m_Register.m_Base] & ((1ull << 32) - 1);
		} break;
		case ILOperandScale_64:
		{
			value = state.m_General[rhs.m_Register.m_Base];
		} break;
		}
	}
	else if (rhs.m_Type == ILOperandType_Memory)
	{
		if (rhs.m_Memory.m_Base != REG_RSP)
		{
			return;
		}

		switch (rhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_16:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_32:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_64:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		}
	}
	else if (rhs.m_Type == ILOperandType_Value)
	{
		value = rhs.m_Value;
	}

	if (lhs.m_Type == ILOperandType_Register)
	{
		if (lhs.m_Register.m_Type != Register::general)
		{
			return;
		}

		if (lhs.m_Register.m_Base == REG_RSP)
		{
			uint64_t original = state.m_General[lhs.m_Register.m_Base];
			switch (lhs.m_Scale)
			{
			case ILOperandScale_8:
			{
				if (lhs.m_Register.m_BaseHigh)
				{
					*(reinterpret_cast<uint8_t*>(&state.m_General[lhs.m_Register.m_Base]) + 1) = value;
				}
				else
				{
					*reinterpret_cast<uint8_t*>(&state.m_General[lhs.m_Register.m_Base]) = value;
				}
			} break;
			default:
			{
				state.m_General[lhs.m_Register.m_Base] = value;
			} break;
			}

			int64_t difference = (state.m_General[lhs.m_Register.m_Base] - original) / 8;

			if (difference > 0)
			{
				for (uint64_t i = 0; i < difference; i++)
				{
					state.m_Stack.pop_back();
				}
			}
			else
			{
				difference = -difference;
				for (uint64_t i = 0; i < difference; i++)
				{
					state.m_Stack.push_back(0);
				}
			}
		}
		else
		{
			switch (lhs.m_Scale)
			{
			case ILOperandScale_8:
			{
				if (lhs.m_Register.m_BaseHigh)
				{
					*(reinterpret_cast<uint8_t*>(&state.m_General[lhs.m_Register.m_Base]) + 1) = value;
				}
				else
				{
					*reinterpret_cast<uint8_t*>(&state.m_General[lhs.m_Register.m_Base]) = value;
				}
			} break;
			default:
			{
				state.m_General[lhs.m_Register.m_Base] = value;
			} break;
			}
		}
	}
	else if (lhs.m_Type == ILOperandType_Memory &&
		lhs.m_Memory.m_Base == REG_RSP)
	{
		uint64_t index = 0;
		if (lhs.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			index = c_MemoryMultiplier[lhs.m_Memory.m_Scale] * state.m_General[lhs.m_Memory.m_Index];
		}

		void* base = &state.m_Stack[state.m_Stack.size() - (lhs.m_Memory.m_Offset / 8) - 1];
		switch (lhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			*(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) = value;
		} break;
		case ILOperandScale_16:
		{
			*reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) = value;
		} break;
		case ILOperandScale_32:
		{
			*reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) = value;
		} break;
		case ILOperandScale_64:
		{
			*reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) = value;
		} break;
		}
	}
	else
	{
		return;
	}
}

void FunctionExplorer::HandleAdd(const ILInstruction& instruction, State& state)
{
	const ILOperand& lhs = instruction.m_Operands[0];
	const ILOperand& rhs = instruction.m_Operands[1];

	int64_t value;
	if (rhs.m_Type == ILOperandType_Register)
	{
		if (rhs.m_Register.m_Type != Register::general)
		{
			return;
		}

		switch (rhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			if (rhs.m_Register.m_BaseHigh)
			{
				value = static_cast<int8_t>((state.m_General[rhs.m_Register.m_Base] >> 8) & ((1ull << 8) - 1));
			}
			else
			{
				value = static_cast<int8_t>(state.m_General[rhs.m_Register.m_Base] & ((1ull << 8) - 1));
			}
		} break;
		case ILOperandScale_16:
		{
			value = static_cast<int16_t>(state.m_General[rhs.m_Register.m_Base] & ((1ull << 16) - 1));
		} break;
		case ILOperandScale_32:
		{
			value = static_cast<int32_t>(state.m_General[rhs.m_Register.m_Base] & ((1ull << 32) - 1));
		} break;
		case ILOperandScale_64:
		{
			value = static_cast<int64_t>(state.m_General[rhs.m_Register.m_Base]);
		} break;
		}
	}
	else if (rhs.m_Type == ILOperandType_Memory)
	{
		if (rhs.m_Memory.m_Base != REG_RSP)
		{
			return;
		}

		switch (rhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *(reinterpret_cast<int8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_16:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<int16_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_32:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_64:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<int64_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		}
	}
	else if (rhs.m_Type == ILOperandType_Value)
	{
		value = rhs.m_Value;
	}

	if (lhs.m_Type == ILOperandType_Register)
	{
		if (lhs.m_Register.m_Type != Register::general)
		{
			return;
		}

		switch (lhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			if (lhs.m_Register.m_BaseHigh)
			{
				*(reinterpret_cast<int8_t*>(&state.m_General[lhs.m_Register.m_Base]) + 1) += value;
			}
			else
			{
				*reinterpret_cast<int8_t*>(&state.m_General[lhs.m_Register.m_Base]) += value;
			}
		} break;
		case ILOperandScale_16:
		{
			*reinterpret_cast<int16_t*>(&state.m_General[lhs.m_Register.m_Base]) += value;
			state.m_General[lhs.m_Register.m_Base] &= (1ull << 16) - 1;
		} break;
		case ILOperandScale_32:
		{
			*reinterpret_cast<int32_t*>(&state.m_General[lhs.m_Register.m_Base]) += value;
			state.m_General[lhs.m_Register.m_Base] &= (1ull << 32) - 1;
		} break;
		case ILOperandScale_64:
		{
			*reinterpret_cast<int64_t*>(&state.m_General[lhs.m_Register.m_Base]) += value;
		} break;
		}

		if (lhs.m_Register.m_Base == REG_RSP)
		{
			if (value > 0)
			{
				for (uint64_t i = 0; i < value / 8; i++)
				{
					state.m_Stack.pop_back();
				}
			}
			else
			{
				value = -value;
				for (uint64_t i = 0; i < value / 8; i++)
				{
					state.m_Stack.push_back(0);
				}
			}
		}
	}
	else if (lhs.m_Type == ILOperandType_Memory &&
		lhs.m_Memory.m_Base == REG_RSP)
	{
		uint64_t index = 0;
		if (lhs.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			index = c_MemoryMultiplier[lhs.m_Memory.m_Scale] * state.m_General[lhs.m_Memory.m_Index];
		}

		void* base = &state.m_Stack[state.m_Stack.size() - (lhs.m_Memory.m_Offset / 8) - 1];
		switch (lhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			*(reinterpret_cast<int8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) += value;
		} break;
		case ILOperandScale_16:
		{
			*reinterpret_cast<int16_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) += value;
		} break;
		case ILOperandScale_32:
		{
			*reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) += value;
		} break;
		case ILOperandScale_64:
		{
			*reinterpret_cast<int64_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) += value;
		} break;
		}
	}
	else
	{
		return;
	}
}

void FunctionExplorer::HandleSub(const ILInstruction& instruction, State& state)
{
	const ILOperand& lhs = instruction.m_Operands[0];
	const ILOperand& rhs = instruction.m_Operands[1];

	int64_t value;
	if (rhs.m_Type == ILOperandType_Register)
	{
		if (rhs.m_Register.m_Type != Register::general)
		{
			return;
		}

		switch (rhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			if (rhs.m_Register.m_BaseHigh)
			{
				value = static_cast<int8_t>((state.m_General[rhs.m_Register.m_Base] >> 8) & ((1ull << 8) - 1));
			}
			else
			{
				value = static_cast<int8_t>(state.m_General[rhs.m_Register.m_Base] & ((1ull << 8) - 1));
			}
		} break;
		case ILOperandScale_16:
		{
			value = static_cast<int16_t>(state.m_General[rhs.m_Register.m_Base] & ((1ull << 16) - 1));
		} break;
		case ILOperandScale_32:
		{
			value = static_cast<int32_t>(state.m_General[rhs.m_Register.m_Base] & ((1ull << 32) - 1));
		} break;
		case ILOperandScale_64:
		{
			value = static_cast<int64_t>(state.m_General[rhs.m_Register.m_Base]);
		} break;
		}
	}
	else if (rhs.m_Type == ILOperandType_Memory)
	{
		if (rhs.m_Memory.m_Base != REG_RSP)
		{
			return;
		}

		switch (rhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *(reinterpret_cast<int8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_16:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<int16_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_32:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		case ILOperandScale_64:
		{
			uint64_t index = 0;
			if (rhs.m_Memory.m_Index != IL_INVALID_REGISTER)
			{
				index = state.m_General[rhs.m_Memory.m_Index] * c_MemoryMultiplier[rhs.m_Memory.m_Scale];
			}

			value = *reinterpret_cast<int64_t*>(reinterpret_cast<uint8_t*>(&state.m_Stack[state.m_Stack.size() - ((rhs.m_Memory.m_Offset + index) / 8) - 1]) + (rhs.m_Memory.m_Offset + index) % 8);
		} break;
		}
	}
	else if (rhs.m_Type == ILOperandType_Value)
	{
		value = rhs.m_Value;
	}

	if (lhs.m_Type == ILOperandType_Register)
	{
		if (lhs.m_Register.m_Type != Register::general)
		{
			return;
		}

		switch (lhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			if (lhs.m_Register.m_BaseHigh)
			{
				*(reinterpret_cast<int8_t*>(&state.m_General[lhs.m_Register.m_Base]) + 1) -= value;
			}
			else
			{
				*reinterpret_cast<int8_t*>(&state.m_General[lhs.m_Register.m_Base]) -= value;
			}
		} break;
		case ILOperandScale_16:
		{
			*reinterpret_cast<int16_t*>(&state.m_General[lhs.m_Register.m_Base]) -= value;
			state.m_General[lhs.m_Register.m_Base] &= (1ull << 16) - 1;
		} break;
		case ILOperandScale_32:
		{
			*reinterpret_cast<int32_t*>(&state.m_General[lhs.m_Register.m_Base]) -= value;
			state.m_General[lhs.m_Register.m_Base] &= (1ull << 32) - 1;
		} break;
		case ILOperandScale_64:
		{
			*reinterpret_cast<int64_t*>(&state.m_General[lhs.m_Register.m_Base]) -= value;
		} break;
		}

		if (lhs.m_Register.m_Base == REG_RSP)
		{
			if (value > 0)
			{
				for (uint64_t i = 0; i < value / 8; i++)
				{
					state.m_Stack.push_back(0);
				}
			}
			else
			{
				value = -value;
				for (uint64_t i = 0; i < value / 8; i++)
				{
					state.m_Stack.pop_back();
				}
			}
		}
	}
	else if (lhs.m_Type == ILOperandType_Memory &&
		lhs.m_Memory.m_Base == REG_RSP)
	{
		uint64_t index = 0;
		if (lhs.m_Memory.m_Index != IL_INVALID_REGISTER)
		{
			index = c_MemoryMultiplier[lhs.m_Memory.m_Scale] * state.m_General[lhs.m_Memory.m_Index];
		}

		void* base = &state.m_Stack[state.m_Stack.size() - (lhs.m_Memory.m_Offset / 8) - 1];
		switch (lhs.m_Scale)
		{
		case ILOperandScale_8:
		{
			*(reinterpret_cast<int8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) -= value;
		} break;
		case ILOperandScale_16:
		{
			*reinterpret_cast<int16_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) -= value;
		} break;
		case ILOperandScale_32:
		{
			*reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) -= value;
		} break;
		case ILOperandScale_64:
		{
			*reinterpret_cast<int64_t*>(reinterpret_cast<uint8_t*>(base) + (lhs.m_Memory.m_Offset % 8)) -= value;
		} break;
		}
	}
	else
	{
		return;
	}
}