#pragma once
#include "PCInstruction.hpp"
#include <vector>

struct PCBlob
{
	std::vector<PCInstruction> m_Instructions;
};