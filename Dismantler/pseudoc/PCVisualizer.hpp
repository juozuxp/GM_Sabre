#pragma once
#include "PCBlob.hpp"
#include <string>

class PCVisualizer
{
public:
	void ToConsole(const PCBlob& blob);
	std::wstring ToString(const PCBlob& blob);

private:
	std::wstring ExpressionToString(const PCExpression& expression);
};