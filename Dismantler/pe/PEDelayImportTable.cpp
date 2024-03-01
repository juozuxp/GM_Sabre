#include "PEDelayImportTable.hpp"

PEDelayImportTable::PEDelayImportTable(const void* base, const IMAGE_DELAYLOAD_DESCRIPTOR* descriptor)
{
	m_Descriptor = *descriptor;
	m_Name = ManagedString(reinterpret_cast<const char*>(reinterpret_cast<const uint64_t*>(reinterpret_cast<uintptr_t>(base) + descriptor->DllNameRVA)));

	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(reinterpret_cast<uintptr_t>(base));
	const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(reinterpret_cast<uintptr_t>(base) + dos->e_lfanew);

	if (nt->FileHeader.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER64))
	{
		const IMAGE_THUNK_DATA64* importName = reinterpret_cast<const IMAGE_THUNK_DATA64*>(reinterpret_cast<uintptr_t>(base) + descriptor->ImportNameTableRVA);
		const IMAGE_THUNK_DATA64* importEntry = reinterpret_cast<const IMAGE_THUNK_DATA64*>(reinterpret_cast<uintptr_t>(base) + descriptor->ImportAddressTableRVA);

		for (; importName->u1.AddressOfData != 0; importName++, importEntry++)
		{
			if (IMAGE_SNAP_BY_ORDINAL64(importName->u1.AddressOfData))
			{
				m_Entries.Add(PEImportEntry(IMAGE_ORDINAL64(importName->u1.Ordinal), reinterpret_cast<uintptr_t>(importEntry) - reinterpret_cast<uintptr_t>(base)));
			}
			else
			{
				const IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<const IMAGE_IMPORT_BY_NAME*>(reinterpret_cast<uintptr_t>(base) + importName->u1.AddressOfData);

				m_Entries.Add(PEImportEntry(name, reinterpret_cast<uintptr_t>(importEntry) - reinterpret_cast<uintptr_t>(base)));
			}
		}
	}
	else
	{
		const IMAGE_THUNK_DATA32* importName = reinterpret_cast<const IMAGE_THUNK_DATA32*>(reinterpret_cast<uintptr_t>(base) + descriptor->ImportNameTableRVA);
		const IMAGE_THUNK_DATA32* importEntry = reinterpret_cast<const IMAGE_THUNK_DATA32*>(reinterpret_cast<uintptr_t>(base) + descriptor->ImportAddressTableRVA);

		for (; importName->u1.AddressOfData != 0; importName++, importEntry++)
		{
			if (IMAGE_SNAP_BY_ORDINAL32(importName->u1.AddressOfData))
			{
				m_Entries.Add(PEImportEntry(IMAGE_ORDINAL32(importName->u1.Ordinal), reinterpret_cast<uintptr_t>(importEntry) - reinterpret_cast<uintptr_t>(base)));
			}
			else
			{
				const IMAGE_IMPORT_BY_NAME* name = reinterpret_cast<const IMAGE_IMPORT_BY_NAME*>(reinterpret_cast<uintptr_t>(base) + importName->u1.AddressOfData);

				m_Entries.Add(PEImportEntry(name, reinterpret_cast<uintptr_t>(importEntry) - reinterpret_cast<uintptr_t>(base)));
			}
		}
	}
}