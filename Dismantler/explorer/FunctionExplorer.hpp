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
		uint64_t m_General[16] = {};

		std::vector<uint64_t> m_Stack;
	};

public:
	std::vector<Function> ExploreFunction(const void* function) const;
	std::vector<Function> ExplorePEFunctions(const PEBuffer& buffer) const;

private:
	std::vector<Function> ExploreBranch(const void* base, const void* branch, State state, std::unordered_set<const void*>& explored, uint32_t& size) const;

private:
	static void HandleMov(const ILInstruction& instruction, State& state);
	static void HandleAdd(const ILInstruction& instruction, State& state);
	static void HandleSub(const ILInstruction& instruction, State& state);
	static void HandleXor(const ILInstruction& instruction, State& state);
	static void HandleAnd(const ILInstruction& instruction, State& state);
	static void HandleOr(const ILInstruction& instruction, State& state);
	static void HandleNot(const ILInstruction& instruction, State& state);

private:
	Disassembler m_Disassembler;

private:
	static constexpr uint8_t c_MemoryMultiplier[] = { 1, 2, 4, 8 };
};