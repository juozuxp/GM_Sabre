#pragma once
#include <string_view>

#include "pe/PEBuffer.hpp"
#include "managed/ManagedObject.hpp"

#include "FunctionExplorer.hpp"
#include "ExecutableView.hpp"

#include "pseudoc/PCVisualizer.hpp"
#include "pseudoc/PCConverter.hpp"

class ExecutableExplorer : public ManagedObject
{
public:
	struct ManagedFunction : public ManagedObject
	{
		const void* m_Base;
		uint32_t m_Size;
		ManagedString m_Name;
	};

public:
	ExecutableExplorer() = default;
	ExecutableExplorer(const std::wstring_view& path);

public:
	PEHeaders* GetHeaders() const;
	ExecutableView* GetExecutableView() const;
	ManagedString* GetPCFunction(uintptr_t function) const;
	ManagedArray<ManagedFunction>* GetExecutableFunctions();

	const PEBuffer& GetBuffer() const;

private:
	PEBuffer m_Buffer;

	Visualizer m_Visualizer;
	Disassembler m_Disassembler;

	FunctionExplorer m_FunctionExplorer;

	PCConverter m_PCCoverter;
	PCVisualizer m_PCVisualizer;
};