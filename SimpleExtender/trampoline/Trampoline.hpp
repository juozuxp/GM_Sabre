#pragma once
#include <stdint.h>

struct Registers
{
	uint64_t m_R15;
	uint64_t m_R14;
	uint64_t m_R13;
	uint64_t m_R12;
	uint64_t m_R11;
	uint64_t m_R10;
	uint64_t m_R9;
	uint64_t m_R8;

	uint64_t m_RDI;
	uint64_t m_RSI;
	uint64_t m_RBP;
	uint64_t m_RBX;
	uint64_t m_RDX;
	uint64_t m_RCX;
	uint64_t m_RAX;
	uint64_t m_RSP; // raed only no need for weird stuff :(((
};

class Trampoline
{
public:
	using Handler = uint64_t(__stdcall*)(Registers& registers);

public:
	Trampoline() = default;
	Trampoline(Handler handler);

public:
	void Hook(void* address);

private:
	static void* AllocateClose(void* address, size_t size);

private:
	Handler m_Handler = nullptr;
	void* m_Trampoline = nullptr;
};