#pragma once
#include <string_view>

#include "pe/PEBuffer.hpp"
#include "managed/ManagedObject.hpp"

#include "FunctionExplorer.hpp"
#include "ExecutableView.hpp"
#include "StringExplorer.hpp"
#include "XRefExplorer.hpp"

#include "pseudoc/PCVisualizer.hpp"
#include "pseudoc/PCConverter.hpp"

class ExecutableExplorer : public ManagedObject
{
public:
	struct ManagedFunction : public ManagedObject
	{
		uint32_t m_Size;
		uintptr_t m_Base;
		ManagedString m_Name;
		ManagedArray<XRefExplorer::Entry> m_XRefs;
	};

	struct ManagedStringEntry : public ManagedObject
	{
		bool m_IsWide;
		uintptr_t m_Base;

		ManagedString m_String;
		ManagedArray<XRefExplorer::Entry> m_XRefs;
	};

public:
	ExecutableExplorer() = default;
	ExecutableExplorer(const std::wstring_view& path);

public:
	PEHeaders* GetHeaders() const;
	ExecutableView* GetExecutableView() const;
	ManagedString* GetPCFunction(uintptr_t function);
	ManagedArray<ManagedFunction>* GetExecutableFunctions();
	ManagedArray<ManagedStringEntry>* GetExecutableStrings();
	ManagedArray<XRefExplorer::Entry>* GetAddressXRefs(uintptr_t address) const;

	const PEBuffer& GetBuffer() const;

private:
	PEBuffer m_Buffer;
	XRefExplorer m_XRefExplorer;

	Visualizer m_Visualizer;
	Disassembler m_Disassembler;

	StringExplorer m_StringExplorer;
	FunctionExplorer m_FunctionExplorer;

	PCConverter m_PCCoverter;
	PCVisualizer m_PCVisualizer;
};