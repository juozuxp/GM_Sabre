#pragma once
#include "kara/KaraConverter.hpp"
#include "pe/PEBuffer.hpp"
#include "PCBlob.hpp"

#include <unordered_map>

class PCConverter
{
private:
	struct State
	{
		KaraBlob m_Kara;
		PCBlob m_Blob;

		uint32_t m_Result;

		std::shared_ptr<PCLine> m_Invoke;
		std::shared_ptr<PCLine> m_Conditional;

		std::vector<uint32_t> m_Arguments;
		std::unordered_map<uint32_t, uint32_t> m_VariableMap;
		std::unordered_map<uint32_t, std::shared_ptr<PCLine>> m_LineMap;
		std::unordered_map<uint32_t, std::vector<std::shared_ptr<PCLine>>> m_GotoMap;
	};

public:
	PCConverter() = default;
	PCConverter(const PEBuffer& buffer);

public:
	PCBlob Convert(uintptr_t function) const;

private:
	PCExpression ConvertExpression(State& state, const KaraOperand& operand) const;

private:
	KaraConverter m_Kara;
};