#include "ExecutableView.hpp"

ExecutableView::ExecutableView(const PEBuffer& buffer, const Disassembler& disassembler, const Visualizer& visualizer)
{
	m_DataAddress = buffer.GetBuffer();

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(m_DataAddress);
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<intptr_t>(dos) + dos->e_lfanew);
	const IMAGE_FILE_HEADER* file = &nt->FileHeader;

	if (file->SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER32))
	{
		const IMAGE_OPTIONAL_HEADER32* optional = &nt->OptionalHeader;

		m_BaseAddress = optional->ImageBase;
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER64* optional = &reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt)->OptionalHeader;

		m_BaseAddress = optional->ImageBase;
	}

	uint32_t firstSection = ~0;

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < file->NumberOfSections; i++, section++)
	{
		if (section->SizeOfRawData == 0)
		{
			continue;
		}

		if (section->VirtualAddress <= firstSection)
		{
			firstSection = section->VirtualAddress;
		}
	}

	ExecutableViewSection peSection;

	peSection.m_Type = ExecutableViewSection::Type::Bytes;

	peSection.m_Start = 0;
	peSection.m_Size = firstSection;

	m_Sections.Add(std::move(peSection));

	section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < file->NumberOfSections; i++, section++)
	{
		if (section->SizeOfRawData == 0)
		{
			continue;
		}

		if (section->Characteristics & IMAGE_SCN_CNT_CODE &&
			file->Machine == IMAGE_FILE_MACHINE_AMD64)
		{
			std::vector<ILInstruction> instructions;
			disassembler.Disassemble(reinterpret_cast<const uint8_t*>(m_DataAddress) + section->VirtualAddress, section->Misc.VirtualSize, instructions);

			uint32_t size = 0;
			for (const ILInstruction& instruction : instructions)
			{
				size += instruction.m_Size;
			}

			ExecutableViewSection codeSection;

			codeSection.m_Type = ExecutableViewSection::Type::Code;

			codeSection.m_Size = size;
			codeSection.m_Start = section->VirtualAddress;
			codeSection.m_Visuals = ManagedGenericArray<Visualizer::Visual>(visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(m_BaseAddress + section->VirtualAddress)));

			m_Sections.Add(std::move(codeSection));

			if (section->SizeOfRawData <= size)
			{
				continue;
			}

			ExecutableViewSection dataSection;

			dataSection.m_Type = ExecutableViewSection::Type::Bytes;

			dataSection.m_Size = section->SizeOfRawData - size;
			dataSection.m_Start = section->VirtualAddress + size;

			m_Sections.Add(std::move(dataSection));
		}
		else
		{
			ExecutableViewSection dataSection;

			dataSection.m_Type = ExecutableViewSection::Type::Bytes;

			dataSection.m_Size = section->SizeOfRawData;
			dataSection.m_Start = section->VirtualAddress;

			m_Sections.Add(std::move(dataSection));
		}
	}
}