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

		std::unordered_map<uint32_t, uint32_t> m_VariableMap;
	};

public:
	PCBlob Convert(const PEBuffer& buffer, uintptr_t function) const;

private:
	PCExpression ConvertExpression(State& state, const KaraOperand& operand) const;

private:
	KaraConverter m_Kara;
};