#include "ExecutableExplorer.hpp"

ExecutableExplorer::ExecutableExplorer(const std::wstring_view& path)
{
	m_Buffer = PEBuffer(path);
	m_XRefExplorer = XRefExplorer(m_Buffer);

	m_PCCoverter = PCConverter(m_Buffer);
	m_PCVisualizer = PCVisualizer(m_Buffer);
	m_StringExplorer = StringExplorer(m_Buffer, m_XRefExplorer);
	m_FunctionExplorer = FunctionExplorer(m_Buffer, m_XRefExplorer);
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
	std::vector<FunctionExplorer::Function> functions = m_FunctionExplorer.ExploreExecutable();
	ManagedArray<ManagedFunction>* array = new ManagedArray<ManagedFunction>(functions.size());

	for (const FunctionExplorer::Function& function : functions)
	{
		ManagedFunction managed;

		managed.m_Base = function.m_Base;
		managed.m_Size = function.m_Size;
		managed.m_Name = ManagedString(function.m_Name);
		managed.m_XRefs = ManagedArray<XRefExplorer::Entry>(function.m_XRefs);

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
		managed.m_XRefs = ManagedArray<XRefExplorer::Entry>(entry.m_XRefs);

		array->Add(std::move(managed));
	}

	return array;
}

ManagedArray<XRefExplorer::Entry>* ExecutableExplorer::GetAddressXRefs(uintptr_t address) const
{
	return new ManagedArray<XRefExplorer::Entry>(m_XRefExplorer.GetXReferences(address));
}

const PEBuffer& ExecutableExplorer::GetBuffer() const
{
	return m_Buffer;
}