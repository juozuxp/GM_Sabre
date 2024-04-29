#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <Disassembler.hpp>
#include <string>

#include "pe/PEBuffer.hpp"
#include "XRefExplorer.hpp"

class FunctionExplorer
{
public:
	struct Function
	{
		Function() = default;
		Function(uintptr_t base, uint32_t size);
		Function(const void* base, uint32_t size);

		uint32_t m_Size;
		uintptr_t m_Base;
		std::wstring m_Name;
		std::vector<XRefExplorer::Entry> m_XRefs;
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
	FunctionExplorer() = default;
	FunctionExplorer(const PEBuffer& buffer, const XRefExplorer& xref);

public:
	std::vector<Function> ExploreExecutable();

private:
	void CollectFunctions(const void* function, std::vector<Function>& functions);
	void ExploreFunction(const void* function, std::vector<Function>& functions);

private:
	void GatherExports(std::vector<const void*>& functions);
	void GatherVirtual(std::vector<const void*>& functions);

private:
	static bool WriteOperand(const ILOperand& operand, State& state, uint64_t value);
	static bool ReadOperand(const ILOperand& operand, const State& state, uint64_t& value);

private:
	const PEBuffer* m_Buffer;
	const XRefExplorer* m_XRefExplorer;

	Disassembler m_Disassembler;

	std::unordered_set<const void*> m_Explored;
	std::unordered_map<uintptr_t, std::wstring> m_Names;

private:
	static constexpr uint8_t c_MemoryMultiplier[] = { 1, 2, 4, 8 };
};