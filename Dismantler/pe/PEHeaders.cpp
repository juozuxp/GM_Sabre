#include "PEHeaders.hpp"

PEHeaders::PEHeaders(const void* base)
{
	m_Dos = *reinterpret_cast<const IMAGE_DOS_HEADER*>(base);

	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(base) + m_Dos.e_lfanew);

	m_NT32 = *nt;
	m_NT64 = *reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt);

	m_Sections = ManagedArray<IMAGE_SECTION_HEADER>(nt->FileHeader.NumberOfSections);

	const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
	for (uint32_t i = 0; i < nt->FileHeader.NumberOfSections; i++, section++)
	{
		m_Sections.Add(*section);
	}

	const IMAGE_FILE_HEADER* file = &m_NT32.FileHeader;

	if (file->SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER64))
	{
		const IMAGE_OPTIONAL_HEADER64* optional = &m_NT64.OptionalHeader;

		m_Base = optional->ImageBase;

		const IMAGE_DATA_DIRECTORY* importDir = &optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

		const IMAGE_IMPORT_DESCRIPTOR* import = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(reinterpret_cast<uintptr_t>(base) + importDir->VirtualAddress);
		for (; import->Characteristics != 0; import++)
		{
			m_Imports.Add(PEImportTable(base, import));
		}
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER32* optional = &m_NT32.OptionalHeader;

		m_Base = optional->ImageBase;

		const IMAGE_DATA_DIRECTORY* importDir = &optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

		const IMAGE_IMPORT_DESCRIPTOR* import = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(reinterpret_cast<uintptr_t>(base) + importDir->VirtualAddress);
		for (; import->Characteristics != 0; import++)
		{
			m_Imports.Add(PEImportTable(base, import));
		}
	}
}