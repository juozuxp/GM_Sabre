#pragma once
#include "PCBlob.hpp"
#include <string>
#include <unordered_map>
#include "pe/PEBuffer.hpp"

class PCVisualizer
{
private:
	struct State
	{
		const PCBlob* m_Blob;

		std::vector<std::wstring> m_VariableNames;
		std::unordered_map<std::shared_ptr<PCLine>, std::wstring> m_LabelNames;
	};

public:
	PCVisualizer() = default;
	PCVisualizer(const PEBuffer& buffer);

public:
	void ToConsole(const PCBlob& blob) const;
	std::wstring ToString(const PCBlob& blob) const;

private:
	bool IsString(uintptr_t address) const;

private:
	std::wstring ExpressionToString(const State& state, const PCExpression& expression) const;

private:
	const PEBuffer* m_Buffer;
};