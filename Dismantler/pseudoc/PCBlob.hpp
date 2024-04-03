#pragma once
#include "PCLine.hpp"
#include "PCVariable.hpp"
#include <memory>

struct PCBlob
{
	std::shared_ptr<PCLine> m_FirstLine;
	std::vector<PCVariable> m_Variables;
};