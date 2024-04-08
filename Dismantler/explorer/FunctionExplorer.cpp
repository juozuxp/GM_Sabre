#include "FunctionExplorer.hpp"
#include <format>

#define CHUNK_SIZE 200

FunctionExplorer::Function::Function(const void* base, uint32_t size) :
	m_Base(base), m_Size(size)
{
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExploreExecutable(const PEBuffer& buffer)
{
	std::vector<FunctionExplorer::Function> found;

	if (buffer.GetFileHeader().Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return std::vector<FunctionExplorer::Function>();
	}

	const IMAGE_OPTIONAL_HEADER64& optional = buffer.GetOptionalHeader64();

	std::vector<const void*> functions;

	GatherExports(buffer, functions);
	GatherVirtual(buffer, functions);

	m_Names[reinterpret_cast<void*>(optional.ImageBase + optional.AddressOfEntryPoint)] = L"entry";
	for (const void* function : functions)
	{
		ExploreFunction(buffer, function, found);
	}

	ExploreFunction(buffer, reinterpret_cast<const uint8_t*>(buffer.GetBuffer()) + optional.AddressOfEntryPoint, found);
	return found;
}

std::vector<FunctionExplorer::Function> FunctionExplorer::ExploreFunction(const PEBuffer& buffer, const void* function)
{
	std::vector<FunctionExplorer::Function> functions;

	ExploreFunction(buffer, function, functions);
	return functions;
}

void FunctionExplorer::ExploreFunction(const PEBuffer& buffer, const void* function, std::vector<Function>& functions)
{
	if (!m_Explored.insert(function).second)
	{
		return;
	}

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.GetBuffer());
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dos) + dos->e_lfanew);

	if (nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return;
	}

	const IMAGE_OPTIONAL_HEADER64* optional = &nt->OptionalHeader;

	uint32_t cursor = functions.size();

	ExploreFunction(function, functions);

	for (uint32_t i = cursor; i < functions.size(); i++)
	{
		functions[i].m_Base = reinterpret_cast<const uint8_t*>(reinterpret_cast<uintptr_t>(functions[i].m_Base) - reinterpret_cast<uintptr_t>(dos) + optional->ImageBase);

		const auto& name = m_Names.find(functions[i].m_Base);
		if (name != m_Names.end())
		{
			functions[i].m_Name = name->second;
		}
		else
		{
			functions[i].m_Name = std::format(L"f_{:016X}", reinterpret_cast<uintptr_t>(functions[i].m_Base));
		}
	}
}

void FunctionExplorer::ExploreFunction(const void* function, std::vector<Function>& functions)
{
	std::vector<StackEntry> stack;
	StackEntry stackEntry;

	State state;

	state.m_General[REG_RSP] = 0x10008;
	state.m_StackBase = state.m_General[REG_RSP];

	stackEntry.m_MaxSize = 0;
	stackEntry.m_AccumSize = 0;
	stackEntry.m_State = state;
	stackEntry.m_Branch = function;
	stackEntry.m_Function = function;

	std::vector<ILInstruction> instructions;
	while (true)
	{
		uint32_t chunkSize = (1 << 12) - (reinterpret_cast<uint64_t>(stackEntry.m_Branch) & ((1 << 12) - 1));
		if (chunkSize > CHUNK_SIZE ||
			chunkSize == 0)
		{
			chunkSize = CHUNK_SIZE;
		}

		instructions.clear();
		m_Disassembler.Disassemble(stackEntry.m_Branch, chunkSize, instructions);

		for (const ILInstruction& instruction : instructions)
		{
			stackEntry.m_AccumSize += instruction.m_Size;
			stackEntry.m_Branch = reinterpret_cast<const uint8_t*>(stackEntry.m_Branch) + instruction.m_Size;

			bool reset = false;

			switch (instruction.m_Type)
			{
			case InsType_jmp:
			{
				const ILOperand& first = instruction.m_Operands[0];

				if (first.m_Type != ILOperandType_ValueRelative)
				{
					if (stackEntry.m_Function != nullptr)
					{
						functions.push_back(Function(stackEntry.m_Function, max(stackEntry.m_AccumSize, stackEntry.m_MaxSize)));
					}

					if (stack.empty())
					{
						return;
					}

					int32_t size = 0;
					if (stackEntry.m_Function == nullptr)
					{
						size = max(stackEntry.m_AccumSize, stackEntry.m_MaxSize);
					}

					stackEntry = stack[stack.size() - 1];
					stack.pop_back();

					stackEntry.m_MaxSize = max(stackEntry.m_MaxSize, size);

					reset = true;
					break;
				}

				const void* address = reinterpret_cast<const uint8_t*>(stackEntry.m_Branch) + first.m_Relative.m_Value;
				if (!m_Explored.insert(address).second)
				{
					if (stackEntry.m_Function != nullptr)
					{
						functions.push_back(Function(stackEntry.m_Function, max(stackEntry.m_AccumSize, stackEntry.m_MaxSize)));
					}

					if (stack.empty())
					{
						return;
					}

					int32_t size = 0;
					if (stackEntry.m_Function == nullptr)
					{
						size = max(stackEntry.m_AccumSize, stackEntry.m_MaxSize);
					}

					stackEntry = stack[stack.size() - 1];
					stack.pop_back();

					stackEntry.m_MaxSize = max(stackEntry.m_MaxSize, size);

					reset = true;
					break;
				}

				if (stackEntry.m_State.m_General[REG_RSP] == stackEntry.m_State.m_StackBase)
				{
					if (stackEntry.m_Function != nullptr)
					{
						functions.push_back(Function(stackEntry.m_Function, max(stackEntry.m_AccumSize, stackEntry.m_MaxSize)));
					}

					stackEntry.m_MaxSize = 0;
					stackEntry.m_AccumSize = 0;
					stackEntry.m_Function = address;
				}
				else
				{
					if (first.m_Relative.m_Value > 0)
					{
						stackEntry.m_AccumSize += first.m_Relative.m_Value;
					}
				}

				stackEntry.m_Branch = address;

				reset = true;
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

				const void* address = reinterpret_cast<const uint8_t*>(stackEntry.m_Branch) + first.m_Relative.m_Value;
				if (!m_Explored.insert(address).second)
				{
					continue;
				}

				stack.push_back(stackEntry);

				stackEntry.m_MaxSize = 0;
				stackEntry.m_AccumSize = 0;
				stackEntry.m_Branch = address;
				stackEntry.m_Function = nullptr;

				reset = true;
			} break;
			case InsType_call:
			{
				const ILOperand& first = instruction.m_Operands[0];

				if (first.m_Type != ILOperandType_ValueRelative)
				{
					continue;
				}

				const void* address = reinterpret_cast<const uint8_t*>(stackEntry.m_Branch) + first.m_Relative.m_Value;
				if (!m_Explored.insert(address).second)
				{
					continue;
				}

				State callState = stackEntry.m_State;

				callState.m_General[REG_RSP] -= 8;
				callState.m_StackBase = callState.m_General[REG_RSP];

				stack.push_back(stackEntry);

				stackEntry.m_MaxSize = 0;
				stackEntry.m_AccumSize = 0;
				stackEntry.m_State = callState;
				stackEntry.m_Branch = address;
				stackEntry.m_Function = address;

				reset = true;
			} break;
			case InsType_mov:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t value;

				if (!ReadOperand(second, stackEntry.m_State, value))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, value))
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
					value = stackEntry.m_State.m_General[second.m_Memory.m_Base];
				}

				if (second.m_Memory.m_Index != IL_INVALID_REGISTER)
				{
					value += stackEntry.m_State.m_General[second.m_Memory.m_Index] * multiplier[second.m_Memory.m_Scale];
				}

				value += second.m_Memory.m_Offset;

				if (!WriteOperand(first, stackEntry.m_State, value))
				{
					break;
				}
			} break;
			case InsType_push:
			{
				stackEntry.m_State.m_General[REG_RSP] -= 8;
			} break;
			case InsType_pop:
			{
				stackEntry.m_State.m_General[REG_RSP] += 8;
			} break;
			case InsType_sub:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, stackEntry.m_State, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, stackEntry.m_State, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, firstValue - secondValue))
				{
					break;
				}
			} break;
			case InsType_add:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, stackEntry.m_State, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, stackEntry.m_State, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, firstValue + secondValue))
				{
					break;
				}
			} break;
			case InsType_xor:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, stackEntry.m_State, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, stackEntry.m_State, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, firstValue ^ secondValue))
				{
					break;
				}
			} break;
			case InsType_and:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, stackEntry.m_State, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, stackEntry.m_State, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, firstValue & secondValue))
				{
					break;
				}
			} break;
			case InsType_or:
			{
				const ILOperand& first = instruction.m_Operands[0];
				const ILOperand& second = instruction.m_Operands[1];

				uint64_t firstValue;
				if (!ReadOperand(first, stackEntry.m_State, firstValue))
				{
					break;
				}

				uint64_t secondValue;
				if (!ReadOperand(second, stackEntry.m_State, secondValue))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, firstValue | secondValue))
				{
					break;
				}
			} break;
			case InsType_not:
			{
				const ILOperand& first = instruction.m_Operands[0];

				uint64_t value;
				if (!ReadOperand(first, stackEntry.m_State, value))
				{
					break;
				}

				if (!WriteOperand(first, stackEntry.m_State, ~value))
				{
					break;
				}
			} break;
			case InsType_ret:
			case InsType_int3:
			case InsType_ud0:
			case InsType_ud1:
			case InsType_ud2:
			case InsType_invalid:
			{
				if (stackEntry.m_Function != nullptr)
				{
					functions.push_back(Function(stackEntry.m_Function, max(stackEntry.m_AccumSize, stackEntry.m_MaxSize)));
				}

				if (stack.empty())
				{
					return;
				}

				int32_t size = 0;
				if (stackEntry.m_Function == nullptr)
				{
					size = max(stackEntry.m_AccumSize, stackEntry.m_MaxSize);
				}

				stackEntry = stack[stack.size() - 1];
				stack.pop_back();

				stackEntry.m_MaxSize = max(stackEntry.m_MaxSize, size);

				reset = true;
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

void FunctionExplorer::GatherExports(const PEBuffer& buffer, std::vector<const void*>& functions)
{
	if (buffer.GetFileHeader().Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return;
	}

	const IMAGE_OPTIONAL_HEADER64& optional = buffer.GetOptionalHeader64();

	const IMAGE_DATA_DIRECTORY* exportDirectory = &optional.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (exportDirectory->Size == 0)
	{
		return;
	}

	const IMAGE_EXPORT_DIRECTORY* exportTable = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + exportDirectory->VirtualAddress);

	const uint32_t* names = reinterpret_cast<const uint32_t*>(reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + exportTable->AddressOfNames);
	const uint32_t* exports = reinterpret_cast<const uint32_t*>(reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + exportTable->AddressOfFunctions);
	const uint16_t* nameOrdinals = reinterpret_cast<const uint16_t*>(reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + exportTable->AddressOfNameOrdinals);
	for (uint16_t i = 0; i < exportTable->NumberOfFunctions; i++, exports++)
	{
		if (*exports >= exportDirectory->VirtualAddress &&
			*exports < (exportDirectory->VirtualAddress + exportDirectory->Size))
		{
			continue;
		}

		const char* name = nullptr;

		const uint32_t* currentName = names;
		const uint16_t* currentNameOrdinal = nameOrdinals;
		for (uint16_t j = 0; j < exportTable->NumberOfNames; j++, currentName++, currentNameOrdinal++)
		{
			if (i == *currentNameOrdinal)
			{
				name = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + *currentName);
				break;
			}
		}

		if (name != nullptr)
		{
			uint32_t length = strlen(name);
			std::wstring wide = std::wstring(length + 1, L'\0');
			size_t converted;

			mbstowcs_s(&converted, wide.data(), wide.size(), name, length);

			wide.erase(wide.size() - 1);
			m_Names[reinterpret_cast<const void*>(optional.ImageBase + *exports)] = wide;
		}
		else
		{
			m_Names[reinterpret_cast<const void*>(optional.ImageBase + *exports)] = std::format(L"export_{:}", exportTable->Base + i);
		}

		functions.push_back(reinterpret_cast<const uint8_t*>(buffer.GetBuffer()) + *exports);
	}
}

void FunctionExplorer::GatherVirtual(const PEBuffer& buffer, std::vector<const void*>& functions)
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(buffer.GetBuffer());
	const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dos) + dos->e_lfanew);
	const IMAGE_FILE_HEADER* file = &nt->FileHeader;

	if (file->Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return;
	}

	const IMAGE_OPTIONAL_HEADER64* optional = &nt->OptionalHeader;

	std::vector<std::pair<uint64_t, uint64_t>> executableArea;

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < file->NumberOfSections; i++, section++)
	{
		if ((section->Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0)
		{
			continue;
		}

		executableArea.push_back(std::pair<uint64_t, uint64_t>(optional->ImageBase + section->VirtualAddress, optional->ImageBase + section->VirtualAddress + section->SizeOfRawData));
	}

	const IMAGE_DATA_DIRECTORY* relocDirectory = &optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if (relocDirectory->Size == 0)
	{
		return;
	}

	const IMAGE_BASE_RELOCATION* baseReloc = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(reinterpret_cast<const uint8_t*>(dos) + relocDirectory->VirtualAddress);
	while (baseReloc->SizeOfBlock != 0)
	{
		uint32_t count = (baseReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(uint16_t);

		const uint16_t* relocInfo = reinterpret_cast<const uint16_t*>(baseReloc + 1);
		const uint8_t* page = reinterpret_cast<const uint8_t*>(dos) + baseReloc->VirtualAddress;
		for (uint32_t i = 0; i < count; i++, relocInfo++)
		{
			uint8_t type = (*relocInfo) >> 12;
			uint16_t offset = (*relocInfo) & ((1 << 12) - 1);

			if (type != IMAGE_REL_BASED_DIR64)
			{
				continue;
			}

			uint64_t address = *reinterpret_cast<const uint64_t*>(page + offset);
			for (const std::pair<uint64_t, uint64_t>& area : executableArea)
			{
				if (address >= area.first &&
					address < area.second)
				{
					functions.push_back(reinterpret_cast<const uint8_t*>(dos) + address - optional->ImageBase);
					break;
				}
			}
		}

		baseReloc = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(reinterpret_cast<const uint8_t*>(baseReloc) + baseReloc->SizeOfBlock);
	}
}