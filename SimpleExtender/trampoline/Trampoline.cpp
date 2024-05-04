#include "Trampoline.hpp"
#include <Windows.h>

#define SHELL_RELATIVITY mainRelativity
#include "utility/x86_x64Shell.h"

Trampoline::Trampoline(Handler handler) :
	m_Handler(handler)
{
}

void* Trampoline::AllocateClose(void* address, size_t size)
{
	SYSTEM_INFO systemInfo;

	GetSystemInfo(&systemInfo);
	
	uintptr_t target = reinterpret_cast<uintptr_t>(address);
	for (uintptr_t runAddress = (target & ~static_cast<uintptr_t>(systemInfo.dwPageSize - 1)) + systemInfo.dwPageSize; (runAddress - target) <= MAXINT32 || (runAddress - target) >= MININT32;)
	{
		MEMORY_BASIC_INFORMATION memoryInfo;
		if (VirtualQuery(reinterpret_cast<void*>(runAddress), &memoryInfo, sizeof(memoryInfo)) == 0)
		{
			break;
		}

		if ((memoryInfo.State & MEM_FREE) == 0)
		{
			runAddress = reinterpret_cast<uintptr_t>(memoryInfo.BaseAddress) + memoryInfo.RegionSize;
			continue;
		}

		runAddress = reinterpret_cast<uintptr_t>(memoryInfo.BaseAddress);
		for (size_t runSize = 0; runSize < memoryInfo.RegionSize; runSize += systemInfo.dwPageSize, runAddress += systemInfo.dwPageSize)
		{
			if ((runAddress - target) >= MAXINT32 && (runAddress - target) <= MININT32)
			{
				return nullptr;
			}

			void* result = VirtualAlloc(reinterpret_cast<void*>(runAddress), size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (result != nullptr)
			{
				return result;
			}
		}
	}

	return nullptr;
}

void Trampoline::Hook(void* address)
{
	uint32_t handlerRelativity = 0;
	uint32_t mainRelativity = 0;

	D_C_S(shell,,
		PUSHQ_R(RSP), // return address
		PUSHQ_R(RAX),
		PUSHQ_R(RCX),
		PUSHQ_R(RDX),
		PUSHQ_R(RBX),
		PUSHQ_R(RBP),
		PUSHQ_R(RSI),
		PUSHQ_R(RDI),

		PUSHQ_R(R8),
		PUSHQ_R(R9),
		PUSHQ_R(R10),
		PUSHQ_R(R11),
		PUSHQ_R(R12),
		PUSHQ_R(R13),
		PUSHQ_R(R14),
		PUSHQ_R(R15),

		PFX_REXW, MOVD_R_RM(R_LR(RBX, RSP)), // backup stack

		MOV_R_D(EAX, 8 - 1),
		PFX_REXW, NOTD_RM(LR(RAX)),
		PFX_REXW, ANDD_R_RM(R_LR(RSP, RAX)), // fix missalignment

		PFX_REXW, SUBD_RM_B(LR(RSP), 0x20),

		PFX_REXW, MOVD_R_RM(R_LR(RCX, RBX)),
		CALLQ_RM(REL_DO(handlerRelativity - mainRelativity)),

		PFX_REXW, MOVD_R_RM(R_LR(RSP, RBX)),

		PFX_REXW, MOVD_RM_R(MRSP_BO_R(RAX, 0x78)),

		POPQ_R(R15),
		POPQ_R(R14),
		POPQ_R(R13),
		POPQ_R(R12),
		POPQ_R(R11),
		POPQ_R(R10),
		POPQ_R(R9),
		POPQ_R(R8),

		POPQ_R(RDI),
		POPQ_R(RSI),
		POPQ_R(RBP),
		POPQ_R(RBX),
		POPQ_R(RDX),
		POPQ_R(RCX),
		POPQ_R(RAX),
		RETN,

		R_CP(handlerRelativity, QWORDPTR(m_Handler))
	);

	m_Trampoline = AllocateClose(address, sizeof(shell));
	if (m_Trampoline == nullptr)
	{
		return;
	}

	memcpy(m_Trampoline, shell, sizeof(shell));

	mainRelativity = 0;

	uint8_t hook[] =
	{
		JMP_RD(reinterpret_cast<uintptr_t>(m_Trampoline) - (reinterpret_cast<uintptr_t>(address) + mainRelativity))
	};

	DWORD oldProtection;
	if (!VirtualProtect(address, sizeof(hook), PAGE_EXECUTE_READWRITE, &oldProtection))
	{
		VirtualFree(m_Trampoline, 0, MEM_FREE);
		return;
	}

	memcpy(address, hook, sizeof(hook));

	VirtualProtect(address, sizeof(hook), oldProtection, &oldProtection);
}
