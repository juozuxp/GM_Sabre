#pragma once
#include "KaraInstruction.hpp"
#include "KaraVariable.hpp"
#include <vector>

struct KaraBlob
{
	std::vector<KaraVariable> m_Variables;
	std::vector<KaraInstruction> m_Instructions;
};