#pragma once
#include <vector>
#include <unordered_set>
#include <Disassembler.hpp>

#include "pe/PEBuffer.hpp"

class FunctionExplorer
{
public:
	struct Function
	{
		Function() = default;
		Function(const void* base, uint32_t size);

		const void* m_Base;
		uint32_t m_Size;
	};

private:
	struct State
	{
		uint64_t m_StackBase = 0;
		uint64_t m_General[16] = {};
	};

	struct StackEntry
	{
		State m_State;

		const void* m_Branch;
		const void* m_Function;

		uint32_t m_MaxSize;
		uint32_t m_AccumSize;
	};

public:
	std::vector<Function> ExploreExecutable(const PEBuffer& buffer);
	std::vector<Function> ExploreFunction(const PEBuffer& buffer, const void* function);

private:
	void ExploreFunction(const void* function, std::vector<Function>& functions);
	void ExploreFunction(const PEBuffer& buffer, const void* function, std::vector<Function>& functions);

private:
	void GatherExports(const PEBuffer& buffer, std::vector<const void*>& functions);
	void GatherVirtual(const PEBuffer& buffer, std::vector<const void*>& functions);

private:
	static bool WriteOperand(const ILOperand& operand, State& state, uint64_t value);
	static bool ReadOperand(const ILOperand& operand, const State& state, uint64_t& value);

private:
	Disassembler m_Disassembler;
	std::unordered_set<const void*> m_Explored;

private:
	static constexpr uint8_t c_MemoryMultiplier[] = { 1, 2, 4, 8 };
};