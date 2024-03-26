#include "PCVisualizer.hpp"

void PCVisualizer::Print(const PCBlob& blob) const
{
	for (const PCInstruction& ins : blob.m_Instructions)
	{
		switch (ins.m_Type)
		{
		case PCInstruction::Type::Assign:
		{
			PrintOperand(blob, ins.m_Operands[0]);

			printf(" = ");

			PrintOperand(blob, ins.m_Operands[1]);
		} break;
		case PCInstruction::Type::Subtract:
		{
			PrintOperand(blob, ins.m_Operands[0]);

			printf(" -= ");

			PrintOperand(blob, ins.m_Operands[1]);
		} break;
		case PCInstruction::Type::Addition:
		{
			PrintOperand(blob, ins.m_Operands[0]);

			printf(" += ");

			PrintOperand(blob, ins.m_Operands[1]);
		} break;
		case PCInstruction::Type::Xor:
		{
			PrintOperand(blob, ins.m_Operands[0]);

			printf(" ^= ");

			PrintOperand(blob, ins.m_Operands[1]);
		} break;
		case PCInstruction::Type::And:
		{
			PrintOperand(blob, ins.m_Operands[0]);

			printf(" &= ");

			PrintOperand(blob, ins.m_Operands[1]);
		} break;
		case PCInstruction::Type::Not:
		{
			PrintOperand(blob, ins.m_Operands[0]);

			printf(" ~= ");

			PrintOperand(blob, ins.m_Operands[0]);
		} break;
		case PCInstruction::Type::Invoke:
		{
			PrintOperand(blob, ins.m_Operands[0]);
			printf("()");
		} break;
		}

		printf("\n");
	}
}

void PCVisualizer::PrintOperand(const PCBlob& blob, const PCOperand& operand) const
{
	switch (operand.m_Type)
	{
	case PCOperand::Type::Variable:
	{
		printf("%s", blob.m_Variables[operand.m_Variable.m_Index - 1].m_Name.c_str());
	} break;
	case PCOperand::Type::Reference:
	{
		printf("&%s", blob.m_Variables[operand.m_Variable.m_Index - 1].m_Name.c_str());
	} break;
	case PCOperand::Type::Expression:
	{
		if (operand.m_Expression.m_BaseVariable != 0)
		{
			printf("%s", blob.m_Variables[operand.m_Expression.m_BaseVariable - 1].m_Name.c_str());
		}

		if (operand.m_Expression.m_IndexVariable != 0)
		{
			if (operand.m_Expression.m_BaseVariable != 0)
			{
				printf(" + ");
			}

			printf("%s", blob.m_Variables[operand.m_Expression.m_IndexVariable - 1].m_Name.c_str());

			if (operand.m_Expression.m_Multiplier >= 1)
			{
				printf(" * %u", operand.m_Expression.m_Multiplier);
			}
		}

		if (operand.m_Expression.m_Offset != 0)
		{
			printf(" %c %u", operand.m_Expression.m_Offset < 0 ? '-' : '+', operand.m_Expression.m_Offset);
		}
	} break;
	case PCOperand::Type::Dereference:
	{
		printf("*(");
		if (operand.m_Expression.m_BaseVariable != 0)
		{
			printf("%s", blob.m_Variables[operand.m_Expression.m_BaseVariable - 1].m_Name.c_str());
		}

		if (operand.m_Expression.m_IndexVariable != 0)
		{
			if (operand.m_Expression.m_BaseVariable != 0)
			{
				printf(" + ");
			}

			printf("%s", blob.m_Variables[operand.m_Expression.m_IndexVariable - 1].m_Name.c_str());

			if (operand.m_Expression.m_Multiplier >= 1)
			{
				printf(" * %u", operand.m_Expression.m_Multiplier);
			}
		}

		if (operand.m_Expression.m_Offset != 0)
		{
			printf(" %c %u", operand.m_Expression.m_Offset < 0 ? '-' : '+', operand.m_Expression.m_Offset);
		}

		printf(")");
	} break;
	case PCOperand::Type::Literal:
	{
		printf("%lld", operand.m_Literal);
	} break;
	}
}