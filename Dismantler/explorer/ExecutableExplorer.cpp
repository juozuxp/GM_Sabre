#include "ExecutableExplorer.hpp"

ExecutableExplorer::ExecutableExplorer(const std::wstring_view& path)
{
	m_Buffer = PEBuffer(path);
}

PEHeaders* ExecutableExplorer::GetHeaders() const
{
	return m_Buffer.GetHeaders();
}

ExecutableView* ExecutableExplorer::GetExecutableView() const
{
	return new ExecutableView(m_Buffer, m_Disassembler, m_Visualizer);
}

void ExecutableExplorer::ExploreFunctions()
{
	m_FunctionExplorer.ExploreExecutable(m_Buffer);
}