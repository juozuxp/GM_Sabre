#include "PEImportTable.hpp"
#include <stdint.h>

PEImportTable::PEImportTable(const void* base, const IMAGE_IMPORT_DESCRIPTOR* descriptor)
{
	m_Descriptor = *descriptor;
	m_Name = ManagedString(reinterpret_cast<const char*>(reinterpret_cast<const uint64_t*>(reinterpret_cast<uintptr_t>(base) + descriptor->Name)));

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(reinterpret_cast<uintptr_t>(base));
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(base) + dos->e_lfanew);

	if (nt->FileHeader.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER64))
	{
		const uint64_t* firstThunk = reinterpret_cast<const uint64_t*>(reinterpret_cast<uintptr_t>(base) + descriptor->FirstThunk);
		const IMAGE_THUNK_DATA64* originalFirstThunk = reinterpret_cast<const IMAGE_THUNK_DATA64*>(reinterpret_cast<uintptr_t>(base) + descriptor->OriginalFirstThunk);

		for (; *firstThunk != 0; firstThunk++, originalFirstThunk++)
		{
			if (IMAGE_SNAP_BY_ORDINAL64(*firstThunk))
			{
				m_Entries.Add(PEImportEntry(IMAGE_ORDINAL64(*firstThunk), reinterpret_cast<uintptr_t>(firstThunk) - reinterpret_cast<uintptr_t>(base), reinterpret_cast<uintptr_t>(originalFirstThunk) - reinterpret_cast<uintptr_t>(base)));
			}
			else
			{
				const IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<const IMAGE_IMPORT_BY_NAME*>(reinterpret_cast<uintptr_t>(base) + *firstThunk);

				m_Entries.Add(PEImportEntry(name, reinterpret_cast<uintptr_t>(firstThunk) - reinterpret_cast<uintptr_t>(base), reinterpret_cast<uintptr_t>(originalFirstThunk) - reinterpret_cast<uintptr_t>(base)));
			}
		}
	}
	else
	{
		const uint32_t* firstThunk = reinterpret_cast<const uint32_t*>(reinterpret_cast<uintptr_t>(base) + descriptor->FirstThunk);
		const IMAGE_THUNK_DATA32* originalFirstThunk = reinterpret_cast<const IMAGE_THUNK_DATA32*>(reinterpret_cast<uintptr_t>(base) + descriptor->OriginalFirstThunk);

		for (; *firstThunk != 0; firstThunk++, originalFirstThunk++)
		{
			if (IMAGE_SNAP_BY_ORDINAL32(*firstThunk))
			{
				m_Entries.Add(PEImportEntry(IMAGE_ORDINAL32(*firstThunk), reinterpret_cast<uintptr_t>(firstThunk) - reinterpret_cast<uintptr_t>(base), reinterpret_cast<uintptr_t>(originalFirstThunk) - reinterpret_cast<uintptr_t>(base)));
			}
			else
			{
				const IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<const IMAGE_IMPORT_BY_NAME*>(reinterpret_cast<uintptr_t>(base) + *firstThunk);

				m_Entries.Add(PEImportEntry(name, reinterpret_cast<uintptr_t>(firstThunk) - reinterpret_cast<uintptr_t>(base), reinterpret_cast<uintptr_t>(originalFirstThunk) - reinterpret_cast<uintptr_t>(base)));
			}
		}
	}
}