#include "StringExplorer.hpp"
#include "utility/CString.hpp"

StringExplorer::StringExplorer(const PEBuffer& buffer, const XRefExplorer& xref) :
	m_Buffer(&buffer), m_XRefExplorer(&xref)
{
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
			entry.m_XRefs = m_XRefExplorer->GetXReferences(entry.m_Base);

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
			entry.m_XRefs = m_XRefExplorer->GetXReferences(entry.m_Base);

			entries.push_back(entry);
		}
	}
}