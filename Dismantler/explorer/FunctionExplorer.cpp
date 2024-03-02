#include "FunctionExplorer.hpp"

#define CHUNK_SIZE 200

FunctionExplorer::Function::Function(const void* base, uint32_t size) :
	m_Base(base), m_Size(size)
{
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExploreExecutable(const PEBuffer& buffer)
{
	std::vector<FunctionExplorer::Function> foundFunctions;

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.GetBuffer());
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dos) + dos->e_lfanew);

	if (nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return std::vector<FunctionExplorer::Function>();
	}

	const IMAGE_OPTIONAL_HEADER64* optional = &nt->OptionalHeader;

	const IMAGE_DATA_DIRECTORY* exportDirectory = &optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (exportDirectory->Size != 0)
	{
		const IMAGE_EXPORT_DIRECTORY* exports = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<uintptr_t>(dos) + exportDirectory->VirtualAddress);

		const uint32_t* functions = reinterpret_cast<const uint32_t*>(reinterpret_cast<uintptr_t>(dos) + exports->AddressOfFunctions);
		for (uint16_t i = 0; i < exports->NumberOfFunctions; i++, functions++)
		{
			if (*functions >= exportDirectory->VirtualAddress &&
				*functions < (exportDirectory->VirtualAddress + exportDirectory->Size))
			{
				continue;
			}

			std::vector<FunctionExplorer::Function> found = ExploreFunction(buffer, reinterpret_cast<const uint8_t*>(dos) + *functions);

			foundFunctions.insert(foundFunctions.begin(), found.begin(), found.end());
		}
	}

	std::vector<FunctionExplorer::Function> found = ExploreFunction(buffer, reinterpret_cast<const uint8_t*>(dos) + optional->AddressOfEntryPoint);

	foundFunctions.insert(foundFunctions.begin(), found.begin(), found.end());
	return foundFunctions;
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExploreFunction(const PEBuffer& buffer, const void* function)
{
	if (!m_Explored.insert(function).second)
	{
		return std::vector<FunctionExplorer::Function>();
	}

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.GetBuffer());
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dos) + dos->e_lfanew);

	if (nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return std::vector<FunctionExplorer::Function>();
	}

	const IMAGE_OPTIONAL_HEADER64* optional = &nt->OptionalHeader;

	std::vector<Function> functions;

	uint32_t size;
	State state;

	state.m_General[REG_RSP] = 0x10008;
	state.m_StackBase = state.m_General[REG_RSP];

	ExploreBranch(function, state, functions, size);

	functions.push_back(Function(function, size));

	for (Function& function : functions)
	{
		function.m_Base = reinterpret_cast<const uint8_t*>(reinterpret_cast<uintptr_t>(function.m_Base) - reinterpret_cast<uintptr_t>(dos) + optional->ImageBase);
	}

	return functions;
}

void FunctionExplorer::ExploreBranch(const void* branch, State state, std::vector<Function>& functions, uint32_t& size)
{
	uint32_t accumulateSize = 0;
	uint32_t maxSize = 0;

	std::vector<ILInstruction> instructions;
	while (true)
	{
		uint32_t chunkSize = (1 << 12) - (reinterpret_cast<uint64_t>(branch) & ((1 << 12) - 1));
		if (chunkSize > CHUNK_SIZE ||
			chunkSize == 0)
		{
			chunkSize = CHUNK_SIZE;
		}

		instructions.clear();
		m_Disassembler.Disassemble(branch, chunkSize, instructions);

		for (const ILInstruction& instruction : instructions)
		{
			accumulateSize += instruction.m_Size;
			branch = reinterpret_cast<const uint8_t*>(branch) + instruction.m_Size;

			bool reset = false;

			switch (instruction.m_Type)
			{
			case InsType_jmp:
			{
				const ILOperand& first = instruction.m_Operands[0];

				if (first.m_Type != ILOperandType_ValueRelative)
				{
					size = max(accumulateSize, maxSize);
					return;
				}

				const void* address = reinterpret_cast<const uint8_t*>(branch) + first.m_Relative.m_Value;
				if (!m_Explored.insert(address).second)
				{
					size = max(accumulateSize, maxSize);
					return;
				}

				if (state.m_General[REG_RSP] == state.m_StackBase)
				{
					uint32_t funcSize = 0;

					ExploreBranch(address, state, functions, funcSize);

					functions.push_back(Function(address, funcSize));

					size = max(accumulateSize, maxSize);
					return;
				}
				else
				{
					if (first.m_Relative.m_Value > 0)
					{
						accumulateSize += first.m_Relative.m_Value;
					}
				}

				reset = true;
				branch = address;
			} break;
			case InsType_ja:
			case InsType_jae:
			case InsType_jb:
			case InsType_jbe:
			case InsType_je:
			case InsType_jg:
			case InsType_jge:
			case InsType_jl:
			case InsType_jle:
			case InsType_jne:
			case InsType_jno:
			case InsType_jns:
			case InsType_jo:
			case InsType_jpe:
			case InsType_jpo:
			case InsType_jrcxz:
			case InsType_js:
			{
				const ILOperand& first = instruction.m_Operands[0];

				if (first.m_Type != ILOperandType_ValueRelative)
				{
					continue;
				}

				const void* address = reinterpret_cast<const uint8_t*>(branch) + first.m_Relative.m_Value;
				if (!m_Explored.insert(address).second)
				{
					continue;
				}

				uint32_t branchSize = 0;

				ExploreBranch(address, state, functions, branchSize);

				if (first.m_Relative.m_Value > 0)
				{
					maxSize = max(branchSize + accumulateSize, maxSize);
				}
			} break;
			case InsType_call:
			{
				const ILOperand& first = instruction.m_Operands[0];

				if (first.m_Type != ILOperandType_ValueRelative)
				{
					continue;
				}

				const void* address = reinterpret_cast<const uint8_t*>(branch) + first.m_Relative.m_Value;
				if (!m_Explored.insert(address).second)
				{
					continue;
				}

				State callState = state;
				uint32_t callSize = 0;

				callState.m_General[REG_RSP] -= 8;
				callState.m_StackBase = callState.m_General[REG_RSP];

				ExploreBranch(address, callState, functions, callSize);

				functions.push_back(Function(address, callSize));
			} break;
			case InsType_mov:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t value;

				if (!ReadOperand(second, state, value))
				{
					break;
				}

				if (!WriteOperand(first, state, value))
				{
					break;
				}
			} break;
			case InsType_lea:
			{
				constexpr uint8_t multiplier[] = { 1, 2, 4, 8 };

				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				if (second.m_Type != ILOperandType_Memory)
				{
					break;
				}

				uint64_t value = 0;

				if (second.m_Memory.m_Base != IL_INVALID_REGISTER)
				{
					value = state.m_General[second.m_Memory.m_Base];
				}

				if (second.m_Memory.m_Index != IL_INVALID_REGISTER)
				{
					value += state.m_General[second.m_Memory.m_Index] * multiplier[second.m_Memory.m_Scale];
				}

				value += second.m_Memory.m_Offset;

				if (!WriteOperand(first, state, value))
				{
					break;
				}
			} break;
			case InsType_push:
			{
				state.m_General[REG_RSP] -= 8;
			} break;
			case InsType_pop:
			{
				state.m_General[REG_RSP] += 8;
			} break;
			case InsType_sub:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, state, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, state, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, state, firstValue - secondValue))
				{
					break;
				}
			} break;
			case InsType_add:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, state, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, state, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, state, firstValue + secondValue))
				{
					break;
				}
			} break;
			case InsType_xor:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, state, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, state, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, state, firstValue ^ secondValue))
				{
					break;
				}
			} break;
			case InsType_and:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, state, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, state, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, state, firstValue & secondValue))
				{
					break;
				}
			} break;
			case InsType_or:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, state, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, state, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, state, firstValue | secondValue))
				{
					break;
				}
			} break;
			case InsType_not:
			{
				const ILOperand& first = instruction.m_Operands[0];

				uint64_t value;
				if (!ReadOperand(first, state, value))
				{
					break;
				}

				if (!WriteOperand(first, state, ~value))
				{
					break;
				}
			} break;
			case InsType_ret:
			{
				size = max(accumulateSize, maxSize);
				return;
			} break;
			}

			if (reset)
			{
				break;
			}
		}
	}
}

bool FunctionExplorer::WriteOperand(const ILOperand& operand, State& state, uint64_t value)
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
				state.m_General[operand.m_Register.m_Base] = (state.m_General[operand.m_Register.m_Base] & ~(((1 << 8) - 1) << 8)) | ((value & ((1 << 8) - 1)) << 8);
			}
			else
			{
				state.m_General[operand.m_Register.m_Base] = (state.m_General[operand.m_Register.m_Base] & ~((1 << 8) - 1)) | (value & ((1 << 8) - 1));
			}

			return true;
		}

		if (operand.m_Scale == ILOperandScale_16)
		{
			state.m_General[operand.m_Register.m_Base] = (state.m_General[operand.m_Register.m_Base] & ~((1 << 16) - 1)) | (value & ((1 << 16) - 1));
			return true;
		}

		state.m_General[operand.m_Register.m_Base] = value & masks[operand.m_Scale];
		return true;
	} break;
	}

	return false;
}

bool FunctionExplorer::ReadOperand(const ILOperand& operand, const State& state, uint64_t& value)
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

		if (operand.m_Scale == ILOperandScale_8 &&
			operand.m_Register.m_BaseHigh)
		{
			value = (state.m_General[operand.m_Register.m_Base] >> 8) & ((1 << 8) - 1);
			return true; 
		}

		value = state.m_General[operand.m_Register.m_Base] & masks[operand.m_Scale];
		return true;
	} break;
	case ILOperandType_Value:
	{
		value = operand.m_Value;
		return true;
	} break;
	}

	return false;
}