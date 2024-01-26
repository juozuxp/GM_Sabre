#include "Visualizer.hpp"
#include "Utility.hpp"

#include <sstream>
#include <iomanip>

void Visualizer::PrintToConsole(const std::vector<ILInstruction>& instructions, const Options& options)
{
	for (const std::wstring& instruction : ToStringArray(instructions, options))
	{
		wprintf(L"%s\n", instruction.c_str());
	}
}

std::vector<std::wstring> Visualizer::ToStringArray(const std::vector<ILInstruction>& instructions, const Options& options)
{
	constexpr std::wstring_view general64[] = { L"rax", L"rcx", L"rdx", L"rbx", L"rsp", L"rbp", L"rsi", L"rdi", L"r8", L"r9", L"r10", L"r11", L"r12", L"r13", L"r14", L"r15" };
	constexpr std::wstring_view general32[] = { L"eax", L"ecx", L"edx", L"ebx", L"esp", L"ebp", L"esi", L"edi", L"r8d", L"r9d", L"r10d", L"r11d", L"r12d", L"r13d", L"r14d", L"r15d" };
	constexpr std::wstring_view general16[] = { L"ax", L"cx", L"dx", L"bx", L"sp", L"bp", L"si", L"di", L"r8w", L"r9w", L"r10w", L"r11w", L"r12w", L"r13w", L"r14w", L"r15w" };
	constexpr std::wstring_view general8L[] = { L"al", L"cl", L"dl", L"bl", L"spl", L"bpl", L"sil", L"dil", L"r8l", L"r9l", L"r10l", L"r11l", L"r12l", L"r13l", L"r14l", L"r15l" };
	constexpr std::wstring_view general8H[] = { L"ah", L"ch", L"dh", L"bh" };

	constexpr std::wstring_view xmm[] = { L"xmm0", L"xmm1", L"xmm2", L"xmm3", L"xmm4", L"xmm5", L"xmm6", L"xmm7", L"xmm8", L"xmm9", L"xmm10", L"xmm11", L"xmm12", L"xmm13", L"xmm14", L"xmm15" };
	constexpr std::wstring_view mm[] = { L"mm0", L"mm1", L"mm2", L"mm3", L"mm4", L"mm5", L"mm6", L"mm7", L"mm8", L"mm9", L"mm10", L"mm11", L"mm12", L"mm13", L"mm14", L"mm15" };
	constexpr std::wstring_view bnd[] = { L"bnd0", L"bnd1", L"bnd2", L"bnd3" };
	constexpr std::wstring_view st[] = { L"st(0)", L"st(1)", L"st(2)", L"st(3)", L"st(4)", L"st(5)", L"st(6)", L"st(7)", L"st(8)", L"st(9)", L"st(10)", L"st(11)", L"st(12)", L"st(13)", L"st(14)", L"st(15)" };
	constexpr std::wstring_view cr[] = { L"cr0", L"cr1", L"cr2", L"cr3", L"cr4", L"cr5", L"cr6", L"cr7", L"cr8", L"cr9", L"cr10", L"cr11", L"cr12", L"cr13", L"cr14", L"cr15" };
	constexpr std::wstring_view dr[] = { L"dr0", L"dr1", L"dr2", L"dr3", L"dr4", L"dr5", L"dr6", L"dr7", L"dr8", L"dr9", L"dr10", L"dr11", L"dr12", L"dr13", L"dr14", L"dr15" };

	constexpr std::wstring_view segment[] = { L"cs", L"ss", L"ds", L"es", L"gs", L"fs" };

	std::wstringstream stream;
	std::vector<std::wstring> strings;

	strings.reserve(instructions.size());
	for (const ILInstruction& instruction : instructions)
	{
		if (instruction.m_Type == InsType_invalid)
		{
			stream << L"invalid";
			continue;
		}

		stream.str(std::wstring());
		stream.clear();

		stream << TypeToName[instruction.m_Type];

		for (uint8_t i = 0; i < ARRAY_SIZE(instruction.m_Operands); i++)
		{
			const ILOperand& operand = instruction.m_Operands[i];

			if (operand.m_Type == ILOperandType_None)
			{
				break;
			}

			if (i != 0)
			{
				stream << L", ";
			}
			else
			{
				stream << L" ";
			}

			switch (operand.m_Type)
			{
			case ILOperandType_Memory:
			{
				if (operand.m_Memory.m_Segment != IL_INVALID_REGISTER)
				{
					stream << segment[operand.m_Memory.m_Segment] << L':';
				}

				stream << L'[';

				if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
				{
					stream << general64[operand.m_Memory.m_Base];
				}

				if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
				{
					if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
					{
						stream << L" + ";
					}

					stream << general64[operand.m_Memory.m_Index];

					switch (operand.m_Memory.m_Scale)
					{
					case ILMemoryScaler_2:
					{
						stream << L" * 2";
					} break;
					case ILMemoryScaler_4:
					{
						stream << L" * 4";
					} break;
					case ILMemoryScaler_8:
					{
						stream << L" * 8";
					} break;
					}
				}

				if (operand.m_Memory.m_Offset != 0)
				{
					bool sign = operand.m_Memory.m_Offset < 0;
					uint32_t value = sign ? -operand.m_Memory.m_Offset : operand.m_Memory.m_Offset;

					stream << (sign ? L" - " : L" + ") << std::hex << std::uppercase << value << L']';
				}
				else
				{
					stream << L']';
				}
			} break;
			case ILOperandType_Register:
			{
				switch (operand.m_Register.m_Type)
				{
				case Register::general:
				{
					switch (operand.m_Scale)
					{
					case ILOperandScale_8:
					{
						if (operand.m_Register.m_BaseHigh)
						{
							stream << general8H[operand.m_Register.m_Base];
						}
						else
						{
							stream << general8L[operand.m_Register.m_Base];
						}
					} break;
					case ILOperandScale_16:
					{
						stream << general16[operand.m_Register.m_Base];
					} break;
					case ILOperandScale_32:
					{
						stream << general32[operand.m_Register.m_Base];
					} break;
					case ILOperandScale_64:
					{
						stream << general64[operand.m_Register.m_Base];
					} break;
					}
				} break;
				case Register::xmm:
				{
					stream << xmm[operand.m_Register.m_Base];
				} break;
				case Register::mm:
				{
					stream << mm[operand.m_Register.m_Base];
				} break;
				case Register::bnd:
				{
					stream << bnd[operand.m_Register.m_Base];
				} break;
				case Register::st:
				{
					stream << st[operand.m_Register.m_Base];
				} break;
				case Register::sreg:
				{
					stream << segment[operand.m_Register.m_Base];
				} break;
				case Register::cr:
				{
					stream << cr[operand.m_Register.m_Base];
				} break;
				case Register::dr:
				{
					stream << dr[operand.m_Register.m_Base];
				} break;
				}
			} break;
			case ILOperandType_Value:
			{
				uint8_t size = 0;
				uint64_t mask = 0;

				switch (operand.m_Scale)
				{
				case ILOperandScale_8:
				{
					size = 2;
					mask = 0xFF;
				} break;
				case ILOperandScale_16:
				{
					size = 4;
					mask = 0xFFFF;
				} break;
				case ILOperandScale_32:
				{
					size = 8;
					mask = 0xFFFFFFFF;
				} break;
				case ILOperandScale_64:
				{
					size = 16;
					mask = 0xFFFFFFFFFFFFFFFF;
				} break;
				}

				stream << std::setfill(L'0') << std::setw(size) << std::hex << std::uppercase << (operand.m_Value & mask);
			} break;
			case ILOperandType_ValueRelative:
			{
				stream << std::setfill(L'0') << std::setw(16) << std::hex << std::uppercase << (reinterpret_cast<const uint8_t*>(options.m_RelativeBase) + operand.m_Value + instruction.m_Size);
			} break;
			case ILOperandType_MemoryRelative:
			{
				if (operand.m_Relative.m_Segment != IL_INVALID_REGISTER)
				{
					stream << segment[operand.m_Relative.m_Segment] << L':';
				}

				stream << L'[' << std::setfill(L'0') << std::setw(16) << std::hex << std::uppercase << (reinterpret_cast<const uint8_t*>(options.m_RelativeBase) + operand.m_Value + instruction.m_Size) << L']';
			} break;
			case ILOperandType_MemoryAbsolute:
			{
				if (operand.m_MemoryValue.m_Segment != IL_INVALID_REGISTER)
				{
					stream << segment[operand.m_MemoryValue.m_Segment] << L':';
				}

				stream << L'[' << std::setfill(L'0') << std::setw(16) << std::hex << std::uppercase << operand.m_MemoryValue.m_Value << L']';
			} break;
			}
		}

		strings.push_back(stream.str());
	}

	return strings;
}