#pragma once
#include "ArchitectureIL.hpp"
#include <string>
#include <vector>

class Visualizer
{
public:
	struct Options
	{
		void* m_RelativeBase;
	};

public:
	void PrintToConsole(const std::vector<ILInstruction>& instructions, const Options& options);

	std::vector<std::wstring> ToStringArray(const std::vector<ILInstruction>& instructions, const Options& options);
};