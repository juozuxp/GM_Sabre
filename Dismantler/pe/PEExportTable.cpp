#include "PEExportTable.hpp"

PEExportTable::PEExportTable(const void* base)
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(reinterpret_cast<uintptr_t>(base));
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(base) + dos->e_lfanew);

	const IMAGE_DATA_DIRECTORY* directory;
	if (nt->FileHeader.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER64))
	{
		directory = &reinterpret_cast<const IMAGE_OPTIONAL_HEADER64*>(&nt->OptionalHeader)->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	}
	else
	{
		directory = &reinterpret_cast<const IMAGE_OPTIONAL_HEADER32*>(&nt->OptionalHeader)->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	}

	m_Descriptor = *reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<uintptr_t>(base) + directory->VirtualAddress);
	m_Name = ManagedString(reinterpret_cast<const char*>(reinterpret_cast<const uint64_t*>(reinterpret_cast<uintptr_t>(base) + m_Descriptor.Name)));

	const uint32_t* names = reinterpret_cast<const uint32_t*>(reinterpret_cast<uintptr_t>(base) + m_Descriptor.AddressOfNames);
	const uint32_t* functions = reinterpret_cast<const uint32_t*>(reinterpret_cast<uintptr_t>(base) + m_Descriptor.AddressOfFunctions);
	const uint16_t* nameOrdinals = reinterpret_cast<const uint16_t*>(reinterpret_cast<uintptr_t>(base) + m_Descriptor.AddressOfNameOrdinals);

	uint16_t ordinal = m_Descriptor.Base;
	for (uint16_t i = 0; i < m_Descriptor.NumberOfFunctions; i++, ordinal++, functions++)
	{
		const char* name = nullptr;

		const uint32_t* currentName = names;
		const uint16_t* currentNameOrdinal = nameOrdinals;
		for (uint16_t j = 0; j < m_Descriptor.NumberOfNames; j++, currentName++, currentNameOrdinal++)
		{
			if (i == *currentNameOrdinal)
			{
				name = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(base) + *currentName);
				break;
			}
		}

		const char* forwarder = nullptr;
		if (*functions >= directory->VirtualAddress && 
			*functions < (directory->VirtualAddress + directory->Size))
		{
			forwarder = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(base) + *functions);
		}

		if (name != nullptr)
		{
			m_Entries.Add(PEExportEntry(ordinal, *functions, name, forwarder));
		}
		else
		{
			m_Entries.Add(PEExportEntry(ordinal, *functions, forwarder));
		}
	}
}