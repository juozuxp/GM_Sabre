#include "ExecutableExplorer.hpp"

ExecutableExplorer::ExecutableExplorer(const std::wstring_view& path)
{
	m_Buffer = PEBuffer(path);
}

PEHeaders* ExecutableExplorer::GetHeaders()
{
	return m_Buffer.GetHeaders();
}