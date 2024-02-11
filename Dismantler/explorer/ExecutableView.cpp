#include "ExecutableView.hpp"

ExecutableView::ExecutableView(const PEBuffer& buffer, const FunctionExplorer& explorer)
{
	explorer.ExplorePEFunctions(buffer);

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
}