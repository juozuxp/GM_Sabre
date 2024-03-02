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

public:
	std::vector<Function> ExploreExecutable(const PEBuffer& buffer);
	std::vector<Function> ExploreFunction(const PEBuffer& buffer, const void* function);

private:
	void ExploreBranch(const void* branch, State state, std::vector<Function>& functions, uint32_t& size);

private:
	static bool WriteOperand(const ILOperand& operand, State& state, uint64_t value);
	static bool ReadOperand(const ILOperand& operand, const State& state, uint64_t& value);

private:
	Disassembler m_Disassembler;
	std::unordered_set<const void*> m_Explored;

private:
	static constexpr uint8_t c_MemoryMultiplier[] = { 1, 2, 4, 8 };
};