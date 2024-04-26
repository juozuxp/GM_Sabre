#include "StringExplorer.hpp"
#include "utility/CString.hpp"

StringExplorer::StringExplorer(const PEBuffer& buffer) :
	m_Buffer(&buffer)
{
	const IMAGE_FILE_HEADER& file = buffer.GetFileHeader();
	if (buffer.GetFileHeader().Machine != IMAGE_FILE_MACHINE_AMD64)
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
			base += instruction.m_Size;
			for (const ILOperand& operand : instruction.m_Operands)
			{
				if (operand.m_Type == ILOperandType_None)
				{
					break;
				}

				if (operand.m_Type == ILOperandType_MemoryRelative)
				{
					m_ReferenceAddresses.push_back(std::pair<uintptr_t, uintptr_t>(base, base + operand.m_Relative.m_Value));
				}
			}
		}
	}
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

			FindCrossReferences(entry);

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

			FindCrossReferences(entry);

			entries.push_back(entry);
		}
	}
}

void StringExplorer::FindCrossReferences(Entry& entry)
{
	for (const std::pair<uintptr_t, uintptr_t>& address : m_ReferenceAddresses)
	{
		if (entry.m_Base == address.second)
		{
			entry.m_CrossReferences.push_back(address.first);
		}
	}
}