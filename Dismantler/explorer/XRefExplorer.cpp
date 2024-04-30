#include "XRefExplorer.hpp"
#include <Disassembler.hpp>

XRefExplorer::Entry::Entry(uintptr_t address, Origin origin) :
	m_Origin(origin), m_Address(address)
{
}

XRefExplorer::XRefExplorer(const PEBuffer& buffer)
{
	if (buffer.GetBuffer() == nullptr)
	{
		return;
	}

	CollectCodeXRefs(buffer);
	CollectDataXRefs(buffer);
}

std::vector<XRefExplorer::Entry> XRefExplorer::GetXReferences(uintptr_t address) const
{
	auto entry = m_XRefs.find(address);
	if (entry == m_XRefs.end())
	{
		return std::vector<Entry>();
	}

	return entry->second;
}

void XRefExplorer::CollectCodeXRefs(const PEBuffer& buffer)
{
	const IMAGE_FILE_HEADER& file = buffer.GetFileHeader();
	if (file.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		return;
	}

	Disassembler dissasembler;

	const IMAGE_OPTIONAL_HEADER64& optional = buffer.GetOptionalHeader64();

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(&buffer.GetNtHeaders64());
	for (uint32_t i = 0; i < file.NumberOfSections; i++, section++)
	{
		if (!(section->Characteristics & IMAGE_SCN_CNT_CODE))
		{
			continue;
		}

		std::vector<ILInstruction> instructions;

		dissasembler.Disassemble(reinterpret_cast<const uint8_t*>(buffer.GetBuffer()) + section->VirtualAddress, section->Misc.VirtualSize, instructions);

		uintptr_t base = optional.ImageBase + section->VirtualAddress;
		for (const ILInstruction& instruction : instructions)
		{
			for (const ILOperand& operand : instruction.m_Operands)
			{
				if (operand.m_Type == ILOperandType_None)
				{
					break;
				}

				if (operand.m_Type == ILOperandType_MemoryRelative ||
					operand.m_Type == ILOperandType_ValueRelative)
				{
					m_XRefs[base + instruction.m_Size + operand.m_Relative.m_Value].push_back(Entry(base, Origin::Code));
				}
				else if (operand.m_Type == ILOperandType_MemoryAbsolute)
				{
					m_XRefs[operand.m_MemoryValue.m_Value].push_back(Entry(base, Origin::Code));
				}
			}

			base += instruction.m_Size;
		}
	}
}

void XRefExplorer::CollectDataXRefs(const PEBuffer& buffer)
{
	const IMAGE_FILE_HEADER& file = buffer.GetFileHeader();

	uintptr_t imageBase;

	const IMAGE_DATA_DIRECTORY* relocDirectory;
	if (file.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER32))
	{
		const IMAGE_OPTIONAL_HEADER32& optional = buffer.GetOptionalHeader32();

		imageBase = optional.ImageBase;
		relocDirectory = &optional.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER64& optional = buffer.GetOptionalHeader64();

		imageBase = optional.ImageBase;
		relocDirectory = &optional.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	}

	if (relocDirectory->Size == 0)
	{
		return;
	}

	const IMAGE_BASE_RELOCATION* baseReloc = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(reinterpret_cast<const uint8_t*>(buffer.GetBuffer()) + relocDirectory->VirtualAddress);
	while (baseReloc->SizeOfBlock != 0)
	{
		uint32_t count = (baseReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(uint16_t);

		const uint16_t* relocInfo = reinterpret_cast<const uint16_t*>(baseReloc + 1);
		const uint8_t* page = reinterpret_cast<const uint8_t*>(buffer.GetBuffer()) + baseReloc->VirtualAddress;
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
				uintptr_t base = reinterpret_cast<uintptr_t>(page + offset) - reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + imageBase;

				m_XRefs[address].push_back(Entry(base, Origin::Data));
			} break;
			case IMAGE_REL_BASED_DIR64:
			{
				uint64_t address = *reinterpret_cast<const uint64_t*>(page + offset);
				uintptr_t base = reinterpret_cast<uintptr_t>(page + offset) - reinterpret_cast<uintptr_t>(buffer.GetBuffer()) + imageBase;

				m_XRefs[address].push_back(Entry(base, Origin::Data));
			} break;
			}

			if (type != IMAGE_REL_BASED_DIR64)
			{
				continue;
			}
		}

		baseReloc = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(reinterpret_cast<const uint8_t*>(baseReloc) + baseReloc->SizeOfBlock);
	}
}