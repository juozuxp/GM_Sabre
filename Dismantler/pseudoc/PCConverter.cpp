#include "PCConverter.hpp"
#include <unordered_map>

PCBlob PCConverter::Convert(const PEBuffer& buffer, uintptr_t function) const
{
	State state;

	m_Kara.Convert(buffer, function, state.m_Kara);

	const PCLine* conditional = nullptr;

	std::shared_ptr<PCLine>* next = &state.m_Blob.m_FirstLine;
	for (const KaraInstruction& ins : state.m_Kara.m_Instructions)
	{
		switch (ins.m_Type)
		{
		case KaraInstruction::Type::Subtract:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Sub;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			*next = line;
			next = &line->m_Next;
		} break;
		case KaraInstruction::Type::Addition:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Add;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			*next = line;
			next = &line->m_Next;
		} break;
		case KaraInstruction::Type::Compare:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = ConvertExpression(state, ins.m_Operands[1]);

			*next = line;
			next = &line->m_Next;
		} break;
		case KaraInstruction::Type::AndCompare:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = ConvertExpression(state, ins.m_Operands[1]);

			*next = line;
			next = &line->m_Next;
		} break;
		case KaraInstruction::Type::Xor:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Xor;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			*next = line;
			next = &line->m_Next;
		} break;
		case KaraInstruction::Type::Or:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Or;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			*next = line;
			next = &line->m_Next;
		} break;
		case KaraInstruction::Type::And:
		{
			std::shared_ptr<PCLine> line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::And;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			*next = line;
			next = &line->m_Next;
		} break;
		}
	}

	return state.m_Blob;
}

PCExpression PCConverter::ConvertExpression(State& state, const KaraOperand& operand) const
{
	constexpr PCVariable::Size sizeMap[] = { PCVariable::Size::int8, PCVariable::Size::int8, PCVariable::Size::int16, PCVariable::Size::int32, PCVariable::Size::int64 };
	constexpr PCVariable::Type typeMap[] = { PCVariable::Type::None, PCVariable::Type::Local, PCVariable::Type::Static, PCVariable::Type::Local, PCVariable::Type::Local };

	PCExpression expression;

	switch (operand.m_Type)
	{
	case KaraOperand::Type::Variable:
	{
		uint32_t& index = state.m_VariableMap[operand.m_Variable.m_Index];
		if (index == 0)
		{
			const KaraVariable& variable = state.m_Kara.m_Variables[operand.m_Variable.m_Index - 1];

			PCVariable cvar;

			cvar.m_Size = sizeMap[variable.m_Size];
			cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

			state.m_Blob.m_Variables.push_back(cvar);

			index = state.m_Blob.m_Variables.size();
		}

		expression.m_Variable = index - 1;
		expression.m_Type = PCExpression::Type::Variable;
	} break;
	case KaraOperand::Type::Reference:
	{
		uint32_t& index = state.m_VariableMap[operand.m_Variable.m_Index];
		if (index == 0)
		{
			const KaraVariable& variable = state.m_Kara.m_Variables[operand.m_Variable.m_Index - 1];

			PCVariable cvar;

			cvar.m_Size = sizeMap[variable.m_Size];
			cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

			state.m_Blob.m_Variables.push_back(cvar);

			index = state.m_Blob.m_Variables.size();
		}

		expression.m_Variable = index - 1;
		expression.m_Type = PCExpression::Type::Reference;
	} break;
	case KaraOperand::Type::Literal:
	{
		expression.m_Literal = operand.m_Literal;
		expression.m_Type = PCExpression::Type::Literal;
	} break;
	case KaraOperand::Type::Expression:
	{
		std::unique_ptr<PCExpression> dereference = std::make_unique<PCExpression>();

		if (operand.m_Expression.m_IndexVariable != 0)
		{
			uint32_t& index = state.m_VariableMap[operand.m_Expression.m_IndexVariable];
			if (index == 0)
			{
				const KaraVariable& variable = state.m_Kara.m_Variables[operand.m_Expression.m_IndexVariable - 1];

				PCVariable cvar;

				cvar.m_Size = sizeMap[variable.m_Size];
				cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

				state.m_Blob.m_Variables.push_back(cvar);

				index = state.m_Blob.m_Variables.size();
			}

			dereference->m_Type = PCExpression::Type::Variable;
			dereference->m_Variable = index;
		}

		if (operand.m_Expression.m_Multiplier != 1)
		{
			std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
			std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

			expRhs->m_Type = PCExpression::Type::Literal;
			expRhs->m_Literal = operand.m_Expression.m_Multiplier;

			dereference->m_Type = PCExpression::Type::Operation;
			dereference->m_Operation.m_Expression = PCExpression::Expression::Mul;

			dereference->m_Operation.m_Left = std::move(expLhs);
			dereference->m_Operation.m_Right = std::move(expRhs);
		}

		if (operand.m_Expression.m_BaseVariable != 0)
		{
			uint32_t& index = state.m_VariableMap[operand.m_Expression.m_BaseVariable];
			if (index == 0)
			{
				const KaraVariable& variable = state.m_Kara.m_Variables[operand.m_Expression.m_BaseVariable - 1];

				PCVariable cvar;

				cvar.m_Size = sizeMap[variable.m_Size];
				cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

				state.m_Blob.m_Variables.push_back(cvar);

				index = state.m_Blob.m_Variables.size();
			}

			if (dereference->m_Type == PCExpression::Type::None)
			{
				dereference->m_Type = PCExpression::Type::Variable;
				dereference->m_Variable = index - 1;
			}
			else
			{
				std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
				std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

				expRhs->m_Type = PCExpression::Type::Variable;
				expRhs->m_Variable = index - 1;

				dereference->m_Type = PCExpression::Type::Operation;
				dereference->m_Operation.m_Expression = PCExpression::Expression::Add;

				dereference->m_Operation.m_Left = std::move(expLhs);
				dereference->m_Operation.m_Right = std::move(expRhs);
			}
		}

		if (operand.m_Expression.m_Offset != 0)
		{
			std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
			std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

			expRhs->m_Type = PCExpression::Type::Literal;
			expRhs->m_Variable = operand.m_Expression.m_Offset;

			dereference->m_Type = PCExpression::Type::Operation;
			dereference->m_Operation.m_Expression = PCExpression::Expression::Add;

			dereference->m_Operation.m_Left = std::move(expLhs);
			dereference->m_Operation.m_Right = std::move(expRhs);
		}

		expression.m_Dereference = std::move(dereference);
		expression.m_Type = PCExpression::Type::Dereference;
	} break;
	case KaraOperand::Type::Dereference:
	{
		if (operand.m_Expression.m_IndexVariable != 0)
		{
			uint32_t& index = state.m_VariableMap[operand.m_Expression.m_IndexVariable];
			if (index == 0)
			{
				const KaraVariable& variable = state.m_Kara.m_Variables[operand.m_Expression.m_IndexVariable - 1];

				PCVariable cvar;

				cvar.m_Size = sizeMap[variable.m_Size];
				cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

				state.m_Blob.m_Variables.push_back(cvar);

				index = state.m_Blob.m_Variables.size();
			}

			expression.m_Type = PCExpression::Type::Variable;
			expression.m_Variable = index;
		}

		if (operand.m_Expression.m_Multiplier != 1)
		{
			std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(expression));
			std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

			expRhs->m_Type = PCExpression::Type::Literal;
			expRhs->m_Literal = operand.m_Expression.m_Multiplier;

			expression.m_Type = PCExpression::Type::Operation;
			expression.m_Operation.m_Expression = PCExpression::Expression::Mul;

			expression.m_Operation.m_Left = std::move(expLhs);
			expression.m_Operation.m_Right = std::move(expRhs);
		}

		if (operand.m_Expression.m_BaseVariable != 0)
		{
			uint32_t& index = state.m_VariableMap[operand.m_Expression.m_BaseVariable];
			if (index == 0)
			{
				const KaraVariable& variable = state.m_Kara.m_Variables[operand.m_Expression.m_BaseVariable - 1];

				PCVariable cvar;

				cvar.m_Size = sizeMap[variable.m_Size];
				cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

				state.m_Blob.m_Variables.push_back(cvar);

				index = state.m_Blob.m_Variables.size();
			}

			if (expression.m_Type == PCExpression::Type::None)
			{
				expression.m_Type = PCExpression::Type::Variable;
				expression.m_Variable = index - 1;
			}
			else
			{
				std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(expression));
				std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

				expRhs->m_Type = PCExpression::Type::Variable;
				expRhs->m_Variable = index - 1;

				expression.m_Type = PCExpression::Type::Operation;
				expression.m_Operation.m_Expression = PCExpression::Expression::Add;

				expression.m_Operation.m_Left = std::move(expLhs);
				expression.m_Operation.m_Right = std::move(expRhs);
			}
		}

		if (operand.m_Expression.m_Offset != 0)
		{
			std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(expression));
			std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

			expRhs->m_Type = PCExpression::Type::Literal;
			expRhs->m_Variable = operand.m_Expression.m_Offset;

			expression.m_Type = PCExpression::Type::Operation;
			expression.m_Operation.m_Expression = PCExpression::Expression::Add;

			expression.m_Operation.m_Left = std::move(expLhs);
			expression.m_Operation.m_Right = std::move(expRhs);
		}
	} break;
	}

	return expression;
}