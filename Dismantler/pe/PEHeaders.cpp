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

		if (optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0)
		{
			const IMAGE_IMPORT_DESCRIPTOR* import = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(reinterpret_cast<uintptr_t>(base) + optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
			for (; import->Name != 0; import++)
			{
				m_Imports.Add(PEImportTable(base, import));
			}
		}

		if (optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size != 0)
		{
			const IMAGE_DELAYLOAD_DESCRIPTOR* delayImport = reinterpret_cast<const IMAGE_DELAYLOAD_DESCRIPTOR*>(reinterpret_cast<uintptr_t>(base) + optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
			for (; delayImport->DllNameRVA != 0; delayImport++)
			{
				m_DelayImports.Add(PEDelayImportTable(base, delayImport));
			}
		}

		if (optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size != 0)
		{
			m_Exports = PEExportTable(base);
		}
	}
	else
	{
		const IMAGE_OPTIONAL_HEADER32* optional = &m_NT32.OptionalHeader;

		m_Base = optional->ImageBase;

		if (optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0)
		{
			const IMAGE_IMPORT_DESCRIPTOR* import = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(reinterpret_cast<uintptr_t>(base) + optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
			for (; import->Name != 0; import++)
			{
				m_Imports.Add(PEImportTable(base, import));
			}
		}

		if (optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size != 0)
		{
			const IMAGE_DELAYLOAD_DESCRIPTOR* delayImport = reinterpret_cast<const IMAGE_DELAYLOAD_DESCRIPTOR*>(reinterpret_cast<uintptr_t>(base) + optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
			for (; delayImport->DllNameRVA != 0; delayImport++)
			{
				m_DelayImports.Add(PEDelayImportTable(base, delayImport));
			}
		}

		if (optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size != 0)
		{
			m_Exports = PEExportTable(base);
		}
	}

}