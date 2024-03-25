#pragma once
#include "PCInstruction.hpp"
#include "PCVariable.hpp"
#include <vector>

struct PCBlob
{
	std::vector<PCVariable> m_Variables;
	std::vector<PCInstruction> m_Instructions;
};