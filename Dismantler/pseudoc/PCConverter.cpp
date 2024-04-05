#include "PCConverter.hpp"
#include <unordered_map>

PCBlob PCConverter::Convert(const PEBuffer& buffer, uintptr_t function) const
{
	State state;

	state.m_Blob.m_Function = function;

	m_Kara.Convert(buffer, function, state.m_Kara);

	std::shared_ptr<PCLine> line;
	for (uint32_t i = 0; i < state.m_Kara.m_Instructions.size(); i++)
	{
		const KaraInstruction& ins = state.m_Kara.m_Instructions[i];
		switch (ins.m_Type)
		{
		case KaraInstruction::Type::Goto:
		{
			line = std::make_shared<PCLine>();

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			line->m_Type = PCLine::Type::Goto;

			uint32_t index = ins.m_Operands[0].m_Literal;

			const auto& jump = state.m_LineMap.find(index);
			if (jump != state.m_LineMap.end())
			{
				line->m_Goto.m_Destination = jump->second;
				state.m_Blob.m_Labels.insert(jump->second);
			}
			else
			{
				state.m_GotoMap[index].push_back(line);
			}

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::GotoE:
		{
			line = std::make_shared<PCLine>();

			if (state.m_Conditional == nullptr)
			{
				break;
			}

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			if (ins.m_Operands[0].m_Literal < i)
			{
				break;
			}

			line->m_Type = PCLine::Type::NotEqual;

			const PCExpression& expression = state.m_Conditional->m_Assign.m_Right;
			switch (expression.m_Operation.m_Expression)
			{
			case PCExpression::Expression::Add:
			case PCExpression::Expression::Mul:
			case PCExpression::Expression::Or:
			case PCExpression::Expression::Xor:
			{
				line->m_Condition.m_Left = expression;

				line->m_Condition.m_Right.m_Literal = 0;
				line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
			} break;
			case PCExpression::Expression::And:
			{
				const PCExpression& lhs = *expression.m_Operation.m_Left;
				const PCExpression& rhs = *expression.m_Operation.m_Right;

				if (lhs.m_Type == rhs.m_Type)
				{
					if (lhs.m_Type != PCExpression::Type::Variable)
					{
						break;
					}

					line->m_Condition.m_Left = lhs;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
				else
				{
					line->m_Condition.m_Left = expression;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
			} break;
			case PCExpression::Expression::Sub:
			{
				line->m_Condition.m_Left = *expression.m_Operation.m_Left;
				line->m_Condition.m_Right = *expression.m_Operation.m_Right;
			} break;
			}

			state.m_GotoMap[ins.m_Operands[0].m_Literal].push_back(line);
			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::GotoNE:
		{
			line = std::make_shared<PCLine>();

			if (state.m_Conditional == nullptr)
			{
				break;
			}

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			if (ins.m_Operands[0].m_Literal < i)
			{
				break;
			}

			line->m_Type = PCLine::Type::Equal;

			const PCExpression& expression = state.m_Conditional->m_Assign.m_Right;
			switch (expression.m_Operation.m_Expression)
			{
			case PCExpression::Expression::Add:
			case PCExpression::Expression::Mul:
			case PCExpression::Expression::Or:
			case PCExpression::Expression::Xor:
			{
				line->m_Condition.m_Left = expression;

				line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				line->m_Condition.m_Right.m_Literal = 0;
			} break;
			case PCExpression::Expression::And:
			{
				const PCExpression& lhs = *expression.m_Operation.m_Left;
				const PCExpression& rhs = *expression.m_Operation.m_Right;

				if (lhs.m_Type == rhs.m_Type)
				{
					if (lhs.m_Type != PCExpression::Type::Variable)
					{
						break;
					}

					line->m_Condition.m_Left = lhs;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
				else
				{
					line->m_Condition.m_Left = expression;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
			} break;
			case PCExpression::Expression::Sub:
			{
				line->m_Condition.m_Left = *expression.m_Operation.m_Left;
				line->m_Condition.m_Right = *expression.m_Operation.m_Right;
			} break;
			}

			state.m_GotoMap[ins.m_Operands[0].m_Literal].push_back(line);
			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::GotoGE:
		{
			line = std::make_shared<PCLine>();

			if (state.m_Conditional == nullptr)
			{
				break;
			}

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			if (ins.m_Operands[0].m_Literal < i)
			{
				break;
			}

			line->m_Type = PCLine::Type::Less;

			const PCExpression& expression = state.m_Conditional->m_Assign.m_Right;
			switch (expression.m_Operation.m_Expression)
			{
			case PCExpression::Expression::Add:
			case PCExpression::Expression::Mul:
			case PCExpression::Expression::Or:
			case PCExpression::Expression::Xor:
			{
				line->m_Type = PCLine::Type::Greater;

				line->m_Condition.m_Left = expression;

				line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				line->m_Condition.m_Right.m_Literal = 0;
			} break;
			case PCExpression::Expression::And:
			{
				line->m_Type = PCLine::Type::Greater;

				const PCExpression& lhs = *expression.m_Operation.m_Left;
				const PCExpression& rhs = *expression.m_Operation.m_Right;

				if (lhs.m_Type == rhs.m_Type)
				{
					if (lhs.m_Type != PCExpression::Type::Variable)
					{
						break;
					}

					line->m_Condition.m_Left = lhs;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
				else
				{
					line->m_Condition.m_Left = expression;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
			} break;
			case PCExpression::Expression::Sub:
			{
				line->m_Condition.m_Left = *expression.m_Operation.m_Left;
				line->m_Condition.m_Right = *expression.m_Operation.m_Right;
			} break;
			}

			state.m_GotoMap[ins.m_Operands[0].m_Literal].push_back(line);
			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::GotoG:
		{
			line = std::make_shared<PCLine>();

			if (state.m_Conditional == nullptr)
			{
				break;
			}

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			if (ins.m_Operands[0].m_Literal < i)
			{
				break;
			}

			line->m_Type = PCLine::Type::LessEqual;

			const PCExpression& expression = state.m_Conditional->m_Assign.m_Right;
			switch (expression.m_Operation.m_Expression)
			{
			case PCExpression::Expression::Add:
			case PCExpression::Expression::Mul:
			case PCExpression::Expression::Or:
			case PCExpression::Expression::Xor:
			{
				line->m_Type = PCLine::Type::GreaterEqual;

				line->m_Condition.m_Left = expression;

				line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				line->m_Condition.m_Right.m_Literal = 0;
			} break;
			case PCExpression::Expression::And:
			{
				line->m_Type = PCLine::Type::GreaterEqual;

				const PCExpression& lhs = *expression.m_Operation.m_Left;
				const PCExpression& rhs = *expression.m_Operation.m_Right;

				if (lhs.m_Type == rhs.m_Type)
				{
					if (lhs.m_Type != PCExpression::Type::Variable)
					{
						break;
					}

					line->m_Condition.m_Left = lhs;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
				else
				{
					line->m_Condition.m_Left = expression;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
			} break;
			case PCExpression::Expression::Sub:
			{
				line->m_Condition.m_Left = *expression.m_Operation.m_Left;
				line->m_Condition.m_Right = *expression.m_Operation.m_Right;
			} break;
			}

			state.m_GotoMap[ins.m_Operands[0].m_Literal].push_back(line);
			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::GotoLE:
		{
			line = std::make_shared<PCLine>();

			if (state.m_Conditional == nullptr)
			{
				break;
			}

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			if (ins.m_Operands[0].m_Literal < i)
			{
				break;
			}

			line->m_Type = PCLine::Type::Greater;

			const PCExpression& expression = state.m_Conditional->m_Assign.m_Right;
			switch (expression.m_Operation.m_Expression)
			{
			case PCExpression::Expression::Add:
			case PCExpression::Expression::Mul:
			case PCExpression::Expression::Or:
			case PCExpression::Expression::Xor:
			{
				line->m_Type = PCLine::Type::Less;

				line->m_Condition.m_Left = expression;

				line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				line->m_Condition.m_Right.m_Literal = 0;
			} break;
			case PCExpression::Expression::And:
			{
				line->m_Type = PCLine::Type::Less;

				const PCExpression& lhs = *expression.m_Operation.m_Left;
				const PCExpression& rhs = *expression.m_Operation.m_Right;

				if (lhs.m_Type == rhs.m_Type)
				{
					if (lhs.m_Type != PCExpression::Type::Variable)
					{
						break;
					}

					line->m_Condition.m_Left = lhs;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
				else
				{
					line->m_Condition.m_Left = expression;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
			} break;
			case PCExpression::Expression::Sub:
			{
				line->m_Condition.m_Left = *expression.m_Operation.m_Left;
				line->m_Condition.m_Right = *expression.m_Operation.m_Right;
			} break;
			}

			state.m_GotoMap[ins.m_Operands[0].m_Literal].push_back(line);
			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::GotoL:
		{
			line = std::make_shared<PCLine>();

			if (state.m_Conditional == nullptr)
			{
				break;
			}

			if (ins.m_Operands[0].m_Type != KaraOperand::Type::Literal)
			{
				break;
			}

			if (ins.m_Operands[0].m_Literal < i)
			{
				break;
			}

			line->m_Type = PCLine::Type::GreaterEqual;

			const PCExpression& expression = state.m_Conditional->m_Assign.m_Right;
			switch (expression.m_Operation.m_Expression)
			{
			case PCExpression::Expression::Add:
			case PCExpression::Expression::Mul:
			case PCExpression::Expression::Or:
			case PCExpression::Expression::Xor:
			{
				line->m_Type = PCLine::Type::LessEqual;

				line->m_Condition.m_Left = expression;

				line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				line->m_Condition.m_Right.m_Literal = 0;
			} break;
			case PCExpression::Expression::And:
			{
				line->m_Type = PCLine::Type::LessEqual;

				const PCExpression& lhs = *expression.m_Operation.m_Left;
				const PCExpression& rhs = *expression.m_Operation.m_Right;

				if (lhs.m_Type == rhs.m_Type)
				{
					if (lhs.m_Type != PCExpression::Type::Variable)
					{
						break;
					}

					line->m_Condition.m_Left = lhs;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
				else
				{
					line->m_Condition.m_Left = expression;

					line->m_Condition.m_Right.m_Literal = 0;
					line->m_Condition.m_Right.m_Type = PCExpression::Type::Literal;
				}
			} break;
			case PCExpression::Expression::Sub:
			{
				line->m_Condition.m_Left = *expression.m_Operation.m_Left;
				line->m_Condition.m_Right = *expression.m_Operation.m_Right;
			} break;
			}

			state.m_GotoMap[ins.m_Operands[0].m_Literal].push_back(line);
			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Invoke: 
		{
			line = std::make_shared<PCLine>();

			line->m_Type = PCLine::Type::Invoke;

			line->m_Invoke.m_Function = ConvertExpression(state, ins.m_Operands[0]);

			for (uint32_t argument : state.m_Arguments)
			{
				if (argument == 0)
				{
					break;
				}

				PCExpression expression;

				expression.m_Type = PCExpression::Type::Variable;
				expression.m_Variable = argument - 1;

				line->m_Invoke.m_Arguments.push_back(expression);
			}

			state.m_Arguments.clear();
			state.m_Invoke = line;
			state.m_Result = 0;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Return:
		{
			line = std::make_shared<PCLine>();

			line->m_Type = PCLine::Type::Return;

			if (state.m_Result != 0)
			{
				line->m_Return.m_Result.m_Type = PCExpression::Type::Variable;
				line->m_Return.m_Result.m_Variable = state.m_Result - 1;
			}

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Assign:
		{
			line = std::make_shared<PCLine>();

			const KaraOperand& lhs = ins.m_Operands[0];
			const KaraOperand& rhs = ins.m_Operands[1];

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, lhs);
			line->m_Assign.m_Right = ConvertExpression(state, rhs);

			if (lhs.m_Type == KaraOperand::Type::Variable)
			{
				const KaraVariable& variable = state.m_Kara.m_Variables[lhs.m_Variable.m_Index - 1];

				if (variable.m_Bind == 1)
				{
					state.m_Result = state.m_VariableMap[lhs.m_Variable.m_Index];
				}
				else if (variable.m_Bind >= 2)
				{
					if (state.m_Arguments.size() < (variable.m_Bind - 1))
					{
						state.m_Arguments.resize(variable.m_Bind - 1);
					}

					state.m_Arguments[variable.m_Bind - 2] = state.m_VariableMap[lhs.m_Variable.m_Index];
				}
			}

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Subtract:
		{
			line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Sub;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			state.m_Conditional = line;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Addition:
		{
			line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Add;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			state.m_Conditional = line;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Not:
		{
			line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Not;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			state.m_Conditional = line;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Compare:
		{
			state.m_Conditional = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Sub;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			state.m_Conditional->m_Type = PCLine::Type::Assign;

			state.m_Conditional->m_Assign.m_Right = std::move(operation);
		} break;
		case KaraInstruction::Type::AndCompare:
		{
			state.m_Conditional = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::And;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			state.m_Conditional->m_Type = PCLine::Type::Assign;

			state.m_Conditional->m_Assign.m_Right = std::move(operation);
		} break;
		case KaraInstruction::Type::Xor:
		{
			line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Xor;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			state.m_Conditional = line;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::Or:
		{
			line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::Or;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			state.m_Conditional = line;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		case KaraInstruction::Type::And:
		{
			line = std::make_shared<PCLine>();

			PCExpression operation;

			operation.m_Type = PCExpression::Type::Operation;

			operation.m_Operation.m_Expression = PCExpression::Expression::And;
			operation.m_Operation.m_Left = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[0])));
			operation.m_Operation.m_Right = std::make_unique<PCExpression>(std::move(ConvertExpression(state, ins.m_Operands[1])));

			line->m_Type = PCLine::Type::Assign;

			line->m_Assign.m_Left = ConvertExpression(state, ins.m_Operands[0]);
			line->m_Assign.m_Right = std::move(operation);

			state.m_Conditional = line;

			state.m_Blob.m_Lines.push_back(line);
		} break;
		}

		if (line == nullptr)
		{
			continue;
		}

		state.m_LineMap[i] = line;

		const auto& jumpCollection = state.m_GotoMap.find(i);
		if (jumpCollection == state.m_GotoMap.end())
		{
			continue;
		}

		for (const std::shared_ptr<PCLine>& jump : jumpCollection->second)
		{
			if (jump->m_Type >= PCLine::Type::Conditional_Start &&
				jump->m_Type <= PCLine::Type::Conditional_End)
			{
				jump->m_Condition.m_Else = line;
			}
			else
			{
				jump->m_Goto.m_Destination = line;
				state.m_Blob.m_Labels.insert(line);
			}
		}

		state.m_GotoMap.erase(i);
	}

	return state.m_Blob;
}

PCExpression PCConverter::ConvertExpression(State& state, const KaraOperand& operand) const
{
	constexpr PCVariable::Size sizeMap[] = { PCVariable::Size::int8, PCVariable::Size::int8, PCVariable::Size::int16, PCVariable::Size::int32, PCVariable::Size::int64 };
	constexpr PCVariable::Type typeMap[] = { PCVariable::Type::None, PCVariable::Type::Local, PCVariable::Type::Static, PCVariable::Type::Local, PCVariable::Type::Argument };

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

			index = state.m_Blob.m_Variables.size() + 1;

			if (variable.m_Type == KaraVariable::Type::Result)
			{
				state.m_Result = index - 1;

				state.m_Invoke->m_Invoke.m_Result.m_Type = PCExpression::Type::Variable;
				state.m_Invoke->m_Invoke.m_Result.m_Variable = index - 1;
			}
			else if (variable.m_Type == KaraVariable::Type::Static)
			{
				cvar.m_Static = variable.m_Address;
			}
			else if (variable.m_Type == KaraVariable::Type::Argument)
			{
				cvar.m_Argument = variable.m_Index;
			}

			state.m_Blob.m_Variables.push_back(cvar);
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

			index = state.m_Blob.m_Variables.size() + 1;

			if (variable.m_Type == KaraVariable::Type::Result)
			{
				state.m_Result = index - 1;

				state.m_Invoke->m_Invoke.m_Result.m_Type = PCExpression::Type::Variable;
				state.m_Invoke->m_Invoke.m_Result.m_Variable = index - 1;
			}
			else if (variable.m_Type == KaraVariable::Type::Static)
			{
				cvar.m_Static = variable.m_Address;
			}
			else if (variable.m_Type == KaraVariable::Type::Argument)
			{
				cvar.m_Argument = variable.m_Index;
			}

			state.m_Blob.m_Variables.push_back(cvar);
		}

		expression.m_Variable = index - 1;
		expression.m_Type = PCExpression::Type::Reference;
	} break;
	case KaraOperand::Type::Literal:
	{
		expression.m_Literal = operand.m_Literal;
		expression.m_Type = PCExpression::Type::Literal;
	} break;
	case KaraOperand::Type::Dereference:
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

				index = state.m_Blob.m_Variables.size() + 1;

				if (variable.m_Type == KaraVariable::Type::Result)
				{
					state.m_Result = index - 1;

					state.m_Invoke->m_Invoke.m_Result.m_Type = PCExpression::Type::Variable;
					state.m_Invoke->m_Invoke.m_Result.m_Variable = index - 1;
				}
				else if (variable.m_Type == KaraVariable::Type::Static)
				{
					cvar.m_Static = variable.m_Address;
				}
				else if (variable.m_Type == KaraVariable::Type::Argument)
				{
					cvar.m_Argument = variable.m_Index;
				}

				state.m_Blob.m_Variables.push_back(cvar);
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

				index = state.m_Blob.m_Variables.size() + 1;

				if (variable.m_Type == KaraVariable::Type::Result)
				{
					state.m_Result = index - 1;

					state.m_Invoke->m_Invoke.m_Result.m_Type = PCExpression::Type::Variable;
					state.m_Invoke->m_Invoke.m_Result.m_Variable = index - 1;
				}
				else if (variable.m_Type == KaraVariable::Type::Static)
				{
					cvar.m_Static = variable.m_Address;
				}
				else if (variable.m_Type == KaraVariable::Type::Argument)
				{
					cvar.m_Argument = variable.m_Index;
				}

				state.m_Blob.m_Variables.push_back(cvar);
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
	case KaraOperand::Type::Expression:
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

				index = state.m_Blob.m_Variables.size() + 1;

				if (variable.m_Type == KaraVariable::Type::Result)
				{
					state.m_Result = index - 1;

					state.m_Invoke->m_Invoke.m_Result.m_Type = PCExpression::Type::Variable;
					state.m_Invoke->m_Invoke.m_Result.m_Variable = index - 1;
				}
				else if (variable.m_Type == KaraVariable::Type::Static)
				{
					cvar.m_Static = variable.m_Address;
				}
				else if (variable.m_Type == KaraVariable::Type::Argument)
				{
					cvar.m_Argument = variable.m_Index;
				}

				state.m_Blob.m_Variables.push_back(cvar);
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

				index = state.m_Blob.m_Variables.size() + 1;

				if (variable.m_Type == KaraVariable::Type::Result)
				{
					state.m_Result = index - 1;

					state.m_Invoke->m_Invoke.m_Result.m_Type = PCExpression::Type::Variable;
					state.m_Invoke->m_Invoke.m_Result.m_Variable = index - 1;
				}
				else if (variable.m_Type == KaraVariable::Type::Static)
				{
					cvar.m_Static = variable.m_Address;
				}
				else if (variable.m_Type == KaraVariable::Type::Argument)
				{
					cvar.m_Argument = variable.m_Index;
				}

				state.m_Blob.m_Variables.push_back(cvar);
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