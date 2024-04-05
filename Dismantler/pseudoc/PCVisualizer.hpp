#pragma once
#include "PCBlob.hpp"
#include <string>
#include <unordered_map>

class PCVisualizer
{
private:
	struct State
	{
		std::vector<std::wstring> m_VariableNames;
		std::unordered_map<std::shared_ptr<PCLine>, std::wstring> m_LabelNames;
	};

public:
	void ToConsole(const PCBlob& blob) const;
	std::wstring ToString(const PCBlob& blob) const;

private:
	std::wstring ExpressionToString(const State& state, const PCExpression& expression) const;
};