#pragma once
#include "PCLine.hpp"
#include "PCVariable.hpp"
#include <memory>
#include <unordered_set>

struct PCBlob
{
	uintptr_t m_Function;

	std::vector<PCVariable> m_Variables;
	std::vector<std::shared_ptr<PCLine>> m_Lines;
	std::unordered_set<std::shared_ptr<PCLine>> m_Labels;
};