#include "StringExplorer.hpp"
#include "utility/CString.hpp"

StringExplorer::CrossReference::CrossReference(uintptr_t base, CrossReferenceOrigin origin) :
	m_Base(base), m_Origin(origin)
{
}

StringExplorer::StringExplorer(const PEBuffer& buffer) :
	m_Buffer(&buffer)
{
	CollectCodeCrossReferences();
	CollectRelocationCrossReferences();
}

std::vector<StringExplorer::Entry> StringExplorer::ExploreExecutable()
{
	std::vector<Entry> entries;

	SearchASCII(entries);
	SearchWide(entries);

	return entries;
}

void StringExplorer::SearchWide(std::vector<Entry>& entries)
{
	const IMAGE_FILE_HEADER& file = m_Buffer->GetFileHeader();

	size_t imageSize = 0;
	uintptr_t imageBase = 0;
	if (file.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER32))
	{
		const IMAGE_OPTIONAL_HEADER32& optional = m_Buffer->GetOptionalHeader32();

		imageBase = optional.ImageBase;
		imageSize = optional.SizeOfImage;
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER64& optional = m_Buffer->GetOptionalHeader64();

		imageBase = optional.ImageBase;
		imageSize = optional.SizeOfImage;
	}

	const void* base = m_Buffer->GetBuffer();
	const void* cursor = m_Buffer->GetBuffer();

	size_t length = 0;
	for (size_t i = 0; i < imageSize; i += (length + 1) * sizeof(wchar_t), cursor = reinterpret_cast<const uint8_t*>(cursor) + ((length + 1) * sizeof(wchar_t)))
	{
		if (m_LowTrustString.GetStringLength(reinterpret_cast<const wchar_t*>(cursor), length))
		{
			if (length < 3)
			{
				continue;
			}

			if (!m_Explored.insert(cursor).second)
			{
				continue;
			}

			Entry entry;

			entry.m_IsWide = true;
			entry.m_String = CString::ConvertReadable(std::wstring_view(reinterpret_cast<const wchar_t*>(cursor), length));
			entry.m_Base = reinterpret_cast<uintptr_t>(cursor) - reinterpret_cast<uintptr_t>(base) + imageBase;

			auto item = m_ReferenceAddresses.find(entry.m_Base);
			if (item != m_ReferenceAddresses.end())
			{
				entry.m_CrossReferences = item->second;
			}

			entries.push_back(entry);
		}
	}
}

void StringExplorer::SearchASCII(std::vector<Entry>& entries)
{
	const IMAGE_FILE_HEADER& file = m_Buffer->GetFileHeader();

	size_t imageSize = 0;
	uintptr_t imageBase = 0;
	if (file.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER32))
	{
		const IMAGE_OPTIONAL_HEADER32& optional = m_Buffer->GetOptionalHeader32();

		imageBase = optional.ImageBase;
		imageSize = optional.SizeOfImage;
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER64& optional = m_Buffer->GetOptionalHeader64();

		imageBase = optional.ImageBase;
		imageSize = optional.SizeOfImage;
	}

	const void* base = m_Buffer->GetBuffer();
	const void* cursor = m_Buffer->GetBuffer();

	size_t length = 0;
	for (size_t i = 0; i < imageSize; i += length + 1, cursor = reinterpret_cast<const uint8_t*>(cursor) + length + 1)
	{
		if (m_LowTrustString.GetStringLength(reinterpret_cast<const char*>(cursor), length))
		{
			if (length < 3)
			{
				continue;
			}

			if (!m_Explored.insert(cursor).second)
			{
				continue;
			}

			std::string cstring = CString::ConvertReadable(std::string_view(reinterpret_cast<const char*>(cursor), length));
			std::wstring wide = std::wstring(length + 1, L'\0');

			size_t converted;

			mbstowcs_s(&converted, wide.data(), wide.size(), cstring.c_str(), length);

			wide.erase(wide.size() - 1);

			Entry entry;

			entry.m_IsWide = false;
			entry.m_String = wide;
			entry.m_Base = reinterpret_cast<uintptr_t>(cursor) - reinterpret_cast<uintptr_t>(base) + imageBase;

			auto item = m_ReferenceAddresses.find(entry.m_Base);
			if (item != m_ReferenceAddresses.end())
			{
				entry.m_CrossReferences = item->second;
			}

			entries.push_back(entry);
		}
	}
}

void StringExplorer::CollectCodeCrossReferences()
{
	const IMAGE_FILE_HEADER& file = m_Buffer->GetFileHeader();
	if (file.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return;
	}

	Disassembler dissasembler;

	const IMAGE_OPTIONAL_HEADER64& optional = m_Buffer->GetOptionalHeader64();

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(&m_Buffer->GetNtHeaders64());
	for (uint32_t i = 0; i < file.NumberOfSections; i++, section++)
	{
		if (!(section->Characteristics & IMAGE_SCN_CNT_CODE))
		{
			continue;
		}

		std::vector<ILInstruction> instructions;

		dissasembler.Disassemble(reinterpret_cast<const uint8_t*>(m_Buffer->GetBuffer()) + section->VirtualAddress, section->Misc.VirtualSize, instructions);

		uintptr_t base = optional.ImageBase + section->VirtualAddress;
		for (const ILInstruction& instruction : instructions)
		{
			base += instruction.m_Size;
			for (const ILOperand& operand : instruction.m_Operands)
			{
				if (operand.m_Type == ILOperandType_None)
				{
					break;
				}

				if (operand.m_Type == ILOperandType_MemoryRelative)
				{
					m_ReferenceAddresses[base + operand.m_Relative.m_Value].push_back(CrossReference(base, CrossReferenceOrigin::Code));
				}
			}
		}
	}
}

void StringExplorer::CollectRelocationCrossReferences()
{
	const IMAGE_FILE_HEADER& file = m_Buffer->GetFileHeader();

	uintptr_t imageBase;

	const IMAGE_DATA_DIRECTORY* relocDirectory;
	if (file.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER32))
	{
		const IMAGE_OPTIONAL_HEADER32& optional = m_Buffer->GetOptionalHeader32();

		imageBase = optional.ImageBase;
		relocDirectory = &optional.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER64& optional = m_Buffer->GetOptionalHeader64();

		imageBase = optional.ImageBase;
		relocDirectory = &optional.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	}

	if (relocDirectory->Size == 0)
	{
		return;
	}

	const IMAGE_BASE_RELOCATION* baseReloc = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(reinterpret_cast<const uint8_t*>(m_Buffer->GetBuffer()) + relocDirectory->VirtualAddress);
	while (baseReloc->SizeOfBlock != 0)
	{
		uint32_t count = (baseReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(uint16_t);

		const uint16_t* relocInfo = reinterpret_cast<const uint16_t*>(baseReloc + 1);
		const uint8_t* page = reinterpret_cast<const uint8_t*>(m_Buffer->GetBuffer()) + baseReloc->VirtualAddress;
		for (uint32_t i = 0; i < count; i++, relocInfo++)
		{
			uint8_t type = (*relocInfo) >> 12;
			uint16_t offset = (*relocInfo) & ((1 << 12) - 1);

			switch (type)
			{
			case IMAGE_REL_BASED_HIGH:
			case IMAGE_REL_BASED_LOW:
			case IMAGE_REL_BASED_HIGHLOW:
			{
				uint32_t address = *reinterpret_cast<const uint32_t*>(page + offset);
				uintptr_t base = reinterpret_cast<uintptr_t>(page + offset) - reinterpret_cast<uintptr_t>(m_Buffer->GetBuffer()) + imageBase;

				m_ReferenceAddresses[address].push_back(CrossReference(base, CrossReferenceOrigin::Data));
			} break;
			case IMAGE_REL_BASED_DIR64:
			{
				uint64_t address = *reinterpret_cast<const uint64_t*>(page + offset);
				uintptr_t base = reinterpret_cast<uintptr_t>(page + offset) - reinterpret_cast<uintptr_t>(m_Buffer->GetBuffer()) + imageBase;

				m_ReferenceAddresses[address].push_back(CrossReference(base, CrossReferenceOrigin::Data));
			} break;
			}

			if (type != IMAGE_REL_BASED_DIR64)
			{
				continue;
			}

			/*
			for (const std::pair<uint64_t, uint64_t>& area : executableArea)
			{
				if (address >= area.first &&
					address < area.second)
				{
					functions.push_back(reinterpret_cast<const uint8_t*>(dos) + address - optional->ImageBase);
					break;
				}
			}*/
		}

		baseReloc = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(reinterpret_cast<const uint8_t*>(baseReloc) + baseReloc->SizeOfBlock);
	}
}