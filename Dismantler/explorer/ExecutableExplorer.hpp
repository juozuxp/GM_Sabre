#pragma once
#include <string_view>

#include "pe/PEBuffer.hpp"
#include "managed/ManagedObject.hpp"

#include "FunctionExplorer.hpp"
#include "ExecutableView.hpp"

class ExecutableExplorer : public ManagedObject
{
public:
	ExecutableExplorer() = default;
	ExecutableExplorer(const std::wstring_view& path);

public:
	PEHeaders* GetHeaders() const;
	ExecutableView* GetExecutableView() const;
	ManagedArray<FunctionExplorer::Function>* GetExecutableFunctions();

private:
	PEBuffer m_Buffer;

	Visualizer m_Visualizer;
	Disassembler m_Disassembler;

	FunctionExplorer m_FunctionExplorer;
};