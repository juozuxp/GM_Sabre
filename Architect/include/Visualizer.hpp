#pragma once
#include "ArchitectureIL.hpp"
#include <string>
#include <vector>

class Visualizer
{
public:
	void PrintToConsole(const std::vector<ILInstruction>& instructions);

	std::vector<std::wstring> ToStringArray(const std::vector<ILInstruction>& instructions);
};