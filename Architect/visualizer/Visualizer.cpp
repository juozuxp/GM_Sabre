#include "Visualizer.hpp"
#include "Utility.hpp"
#include <sstream>

void Visualizer::PrintToConsole(const std::vector<ILInstruction>& instructions)
{
	for (const std::wstring& instruction : ToStringArray(instructions))
	{
		wprintf(L"%s\n", instruction.c_str());
	}
}

std::vector<std::wstring> Visualizer::ToStringArray(const std::vector<ILInstruction>& instructions)
{
	std::wstringstream stream;
	std::vector<std::wstring> strings;

	strings.reserve(instructions.size());
	for (const ILInstruction& instruction : instructions)
	{
		if (instruction.m_Type == InsType_invalid)
		{
			stream << "invalid";
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
				stream << ", ";
			}
			else
			{
				stream << " ";
			}

			switch (operand.m_Type)
			{
			case ILOperandType_Memory:
			{
				if (operand.m_Memory.m_Segment != IL_INVALID_REGISTER)
				{
					stream << 's' << std::dec << operand.m_Memory.m_Segment << ':';
				}

				stream << '[';

				if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
				{
					stream << 'r' << std::dec << operand.m_Memory.m_Base;
				}

				if (operand.m_Memory.m_Index != IL_INVALID_REGISTER)
				{
					if (operand.m_Memory.m_Base != IL_INVALID_REGISTER)
					{
						stream << " + ";
					}

					stream << 'r' << std::dec << operand.m_Memory.m_Index;

					switch (operand.m_Memory.m_Scale)
					{
					case ILMemoryScaler_2:
					{
						stream << " * 2";
					} break;
					case ILMemoryScaler_4:
					{
						stream << " * 4";
					} break;
					case ILMemoryScaler_8:
					{
						stream << " * 8";
					} break;
					}
				}

				if (operand.m_Memory.m_Offset != 0)
				{
					bool sign = operand.m_Memory.m_Offset < 0;
					uint32_t value = sign ? -operand.m_Memory.m_Offset : operand.m_Memory.m_Offset;

					stream << (sign ? " - " : " + ") << std::hex << value << ']';
				}
				else
				{
					stream << ']';
				}
			} break;
			case ILOperandType_Register:
			{
				stream << 'r' << std::dec << operand.m_Register.m_Base;

				if (operand.m_Register.m_BaseHigh)
				{
					stream << 'h';
				}
			} break;
			case ILOperandType_Value:
			{
				stream << std::hex << operand.m_Value;
			} break;
			case ILOperandType_ValueRelative:
			{
				uint32_t value = 0;
				bool sign = false;

				switch (operand.m_Scale)
				{
				case ILOperandScale_8:
				{
					int8_t convert = static_cast<int8_t>(operand.m_Relative.m_Value);

					sign = convert < 0;
					value = sign ? -convert : convert;
				} break;
				case ILOperandScale_16:
				{
					int16_t convert = static_cast<int16_t>(operand.m_Relative.m_Value);

					sign = convert < 0;
					value = sign ? -convert : convert;
				} break;
				case ILOperandScale_32:
				{
					int32_t convert = static_cast<int32_t>(operand.m_Relative.m_Value);

					sign = convert < 0;
					value = sign ? -convert : convert;
				} break;
				}

				stream << "rip" << (sign ? " - " : " + ") << std::hex << value;
			} break;
			case ILOperandType_MemoryRelative:
			{
				if (operand.m_Relative.m_Segment != IL_INVALID_REGISTER)
				{
					stream << 's' << std::dec << operand.m_Relative.m_Segment << ':';
				}

				int32_t convert = static_cast<int32_t>(operand.m_Relative.m_Value);
				bool sign = convert < 0;

				stream << "[rip" << (sign ? " - " : " + ") << std::hex << (sign ? -convert : convert) << ']';
			} break;
			case ILOperandType_MemoryAbsolute:
			{
				if (operand.m_MemoryValue.m_Segment != IL_INVALID_REGISTER)
				{
					stream << 's' << std::dec << operand.m_MemoryValue.m_Segment << ':';
				}

				stream << '[' << std::hex << operand.m_MemoryValue.m_Value << ']';
			} break;
			}
		}

		strings.push_back(stream.str());
	}

	return strings;
}