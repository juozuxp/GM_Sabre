#pragma once
#include "PCLine.hpp"
#include "PCVariable.hpp"

struct PCBlob
{
	std::vector<PCLine> m_Lines;
	std::vector<PCVariable> m_Variables;
};