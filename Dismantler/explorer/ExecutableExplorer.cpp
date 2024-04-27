#include "ExecutableExplorer.hpp"

ExecutableExplorer::ExecutableExplorer(const std::wstring_view& path)
{
	m_Buffer = PEBuffer(path);
	m_PCCoverter = PCConverter(m_Buffer);
	m_PCVisualizer = PCVisualizer(m_Buffer);
	m_StringExplorer = StringExplorer(m_Buffer);
}

PEHeaders* ExecutableExplorer::GetHeaders() const
{
	return m_Buffer.GetHeaders();
}

ExecutableView* ExecutableExplorer::GetExecutableView() const
{
	return new ExecutableView(m_Buffer, m_Disassembler, m_Visualizer);
}

ManagedString* ExecutableExplorer::GetPCFunction(uintptr_t function)
{
	return new ManagedString(m_PCVisualizer.ToString(m_PCCoverter.Convert(function)));
}

ManagedArray<ExecutableExplorer::ManagedFunction>* ExecutableExplorer::GetExecutableFunctions()
{
	std::vector<FunctionExplorer::Function> functions = m_FunctionExplorer.ExploreExecutable(m_Buffer);
	ManagedArray<ManagedFunction>* array = new ManagedArray<ManagedFunction>(functions.size());

	for (const FunctionExplorer::Function& function : functions)
	{
		ManagedFunction managed;

		managed.m_Base = function.m_Base;
		managed.m_Size = function.m_Size;
		managed.m_Name = ManagedString(function.m_Name);

		array->Add(std::move(managed));
	}

	return array;
}

ManagedArray<ExecutableExplorer::ManagedStringEntry>* ExecutableExplorer::GetExecutableStrings()
{
	std::vector<StringExplorer::Entry> entries = m_StringExplorer.ExploreExecutable();
	ManagedArray<ManagedStringEntry>* array = new ManagedArray<ManagedStringEntry>(entries.size());

	for (const StringExplorer::Entry& entry : entries)
	{
		ManagedStringEntry managed;

		managed.m_Base = entry.m_Base;
		managed.m_IsWide = entry.m_IsWide;
		managed.m_String = ManagedString(entry.m_String);
		managed.m_CrossReferences = ManagedArray<StringExplorer::CrossReference>(entry.m_CrossReferences);

		array->Add(std::move(managed));
	}

	return array;
}

const PEBuffer& ExecutableExplorer::GetBuffer() const
{
	return m_Buffer;
}