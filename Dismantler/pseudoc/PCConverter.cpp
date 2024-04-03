#include "PCConverter.hpp"
#include <unordered_map>

void PCConverter::Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const
{
	constexpr PCVariable::Size sizeMap[] = { PCVariable::Size::int8, PCVariable::Size::int8, PCVariable::Size::int16, PCVariable::Size::int32, PCVariable::Size::int64 };
	constexpr PCVariable::Type typeMap[] = { PCVariable::Type::None, PCVariable::Type::Local, PCVariable::Type::Static, PCVariable::Type::Local, PCVariable::Type::Local };

	std::unordered_map<uint32_t, uint32_t> variableMap;

	KaraBlob karaBlob;

	m_Kara.Convert(buffer, function, karaBlob);

	const PCLine* conditional = nullptr;
	for (const KaraInstruction& ins : karaBlob.m_Instructions)
	{
		switch (ins.m_Type)
		{
		case KaraInstruction::Type::Subtract:
		{
			PCLine line;

			line.m_Type = PCLine::Type::Assign;

			PCExpression& lhs = line.m_Assign.m_Left;
			PCExpression& rhs = line.m_Assign.m_Right;

			const KaraOperand& opLhs = ins.m_Operands[0];
			const KaraOperand& opRhs = ins.m_Operands[1];

			switch (opLhs.m_Type)
			{
			case KaraOperand::Type::Variable:
			{
				uint32_t& index = variableMap[opLhs.m_Variable.m_Index];
				if (index == 0)
				{
					const KaraVariable& variable = karaBlob.m_Variables[opLhs.m_Variable.m_Index - 1];

					PCVariable cvar;

					cvar.m_Size = sizeMap[variable.m_Size];
					cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

					blob.m_Variables.push_back(cvar);

					index = blob.m_Variables.size();
				}

				lhs.m_Variable = index - 1;
				lhs.m_Type = PCExpression::Type::Variable;
			} break;
			case KaraOperand::Type::Dereference:
			{
				std::unique_ptr<PCExpression> dereference = std::make_unique<PCExpression>();

				if (opLhs.m_Expression.m_IndexVariable != 0)
				{
					uint32_t& index = variableMap[opLhs.m_Expression.m_IndexVariable];
					if (index == 0)
					{
						const KaraVariable& variable = karaBlob.m_Variables[opLhs.m_Expression.m_IndexVariable - 1];

						PCVariable cvar;

						cvar.m_Size = sizeMap[variable.m_Size];
						cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

						blob.m_Variables.push_back(cvar);

						index = blob.m_Variables.size();
					}

					dereference->m_Type = PCExpression::Type::Variable;
					dereference->m_Variable = index;
				}

				if (opLhs.m_Expression.m_Multiplier != 1)
				{
					std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
					std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

					expRhs->m_Type = PCExpression::Type::Literal;
					expRhs->m_Literal = opLhs.m_Expression.m_Multiplier;

					dereference->m_Type = PCExpression::Type::Operator;
					dereference->m_Operator.m_Expression = PCExpression::Expression::Mul;

					dereference->m_Operator.m_Left = std::move(expLhs);
					dereference->m_Operator.m_Right = std::move(expRhs);
				}

				if (opLhs.m_Expression.m_BaseVariable != 0)
				{
					uint32_t& index = variableMap[opLhs.m_Expression.m_BaseVariable];
					if (index == 0)
					{
						const KaraVariable& variable = karaBlob.m_Variables[opLhs.m_Expression.m_BaseVariable - 1];

						PCVariable cvar;

						cvar.m_Size = sizeMap[variable.m_Size];
						cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

						blob.m_Variables.push_back(cvar);

						index = blob.m_Variables.size();
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

						dereference->m_Type = PCExpression::Type::Operator;
						dereference->m_Operator.m_Expression = PCExpression::Expression::Add;

						dereference->m_Operator.m_Left = std::move(expLhs);
						dereference->m_Operator.m_Right = std::move(expRhs);
					}
				}

				if (opLhs.m_Expression.m_Offset != 0)
				{
					std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
					std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

					expRhs->m_Type = PCExpression::Type::Literal;
					expRhs->m_Variable = opLhs.m_Expression.m_Offset;

					dereference->m_Type = PCExpression::Type::Operator;
					dereference->m_Operator.m_Expression = PCExpression::Expression::Add;

					dereference->m_Operator.m_Left = std::move(expLhs);
					dereference->m_Operator.m_Right = std::move(expRhs);
				}

				lhs.m_Dereference = std::move(dereference);
				lhs.m_Type = PCExpression::Type::Dereference;
			} break;
			}

			switch (opRhs.m_Type)
			{
			case KaraOperand::Type::Variable:
			{
				uint32_t& index = variableMap[opRhs.m_Variable.m_Index];
				if (index == 0)
				{
					const KaraVariable& variable = karaBlob.m_Variables[opRhs.m_Variable.m_Index - 1];

					PCVariable cvar;

					cvar.m_Size = sizeMap[variable.m_Size];
					cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

					blob.m_Variables.push_back(cvar);

					index = blob.m_Variables.size();
				}

				rhs.m_Variable = index - 1;
				rhs.m_Type = PCExpression::Type::Variable;
			} break;
			case KaraOperand::Type::Reference:
			{
				uint32_t& index = variableMap[opRhs.m_Variable.m_Index];
				if (index == 0)
				{
					const KaraVariable& variable = karaBlob.m_Variables[opRhs.m_Variable.m_Index - 1];

					PCVariable cvar;

					cvar.m_Size = sizeMap[variable.m_Size];
					cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

					blob.m_Variables.push_back(cvar);

					index = blob.m_Variables.size();
				}

				rhs.m_Variable = index - 1;
				rhs.m_Type = PCExpression::Type::Reference;
			} break;
			case KaraOperand::Type::Literal:
			{
				rhs.m_Literal = opRhs.m_Literal;
				rhs.m_Type = PCExpression::Type::Literal;
			} break;
			case KaraOperand::Type::Expression:
			{
				std::unique_ptr<PCExpression> dereference = std::make_unique<PCExpression>();

				if (opRhs.m_Expression.m_IndexVariable != 0)
				{
					uint32_t& index = variableMap[opRhs.m_Expression.m_IndexVariable];
					if (index == 0)
					{
						const KaraVariable& variable = karaBlob.m_Variables[opRhs.m_Expression.m_IndexVariable - 1];

						PCVariable cvar;

						cvar.m_Size = sizeMap[variable.m_Size];
						cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

						blob.m_Variables.push_back(cvar);

						index = blob.m_Variables.size();
					}

					dereference->m_Type = PCExpression::Type::Variable;
					dereference->m_Variable = index;
				}

				if (opRhs.m_Expression.m_Multiplier != 1)
				{
					std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
					std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

					expRhs->m_Type = PCExpression::Type::Literal;
					expRhs->m_Literal = opRhs.m_Expression.m_Multiplier;

					dereference->m_Type = PCExpression::Type::Operator;
					dereference->m_Operator.m_Expression = PCExpression::Expression::Mul;

					dereference->m_Operator.m_Left = std::move(expLhs);
					dereference->m_Operator.m_Right = std::move(expRhs);
				}

				if (opRhs.m_Expression.m_BaseVariable != 0)
				{
					uint32_t& index = variableMap[opRhs.m_Expression.m_BaseVariable];
					if (index == 0)
					{
						const KaraVariable& variable = karaBlob.m_Variables[opRhs.m_Expression.m_BaseVariable - 1];

						PCVariable cvar;

						cvar.m_Size = sizeMap[variable.m_Size];
						cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

						blob.m_Variables.push_back(cvar);

						index = blob.m_Variables.size();
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

						dereference->m_Type = PCExpression::Type::Operator;
						dereference->m_Operator.m_Expression = PCExpression::Expression::Add;

						dereference->m_Operator.m_Left = std::move(expLhs);
						dereference->m_Operator.m_Right = std::move(expRhs);
					}
				}

				if (opRhs.m_Expression.m_Offset != 0)
				{
					std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(*dereference));
					std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

					expRhs->m_Type = PCExpression::Type::Literal;
					expRhs->m_Variable = opRhs.m_Expression.m_Offset;

					dereference->m_Type = PCExpression::Type::Operator;
					dereference->m_Operator.m_Expression = PCExpression::Expression::Add;

					dereference->m_Operator.m_Left = std::move(expLhs);
					dereference->m_Operator.m_Right = std::move(expRhs);
				}

				rhs.m_Dereference = std::move(dereference);
				rhs.m_Type = PCExpression::Type::Dereference;
			} break;
			case KaraOperand::Type::Dereference:
			{
				if (opRhs.m_Expression.m_IndexVariable != 0)
				{
					uint32_t& index = variableMap[opRhs.m_Expression.m_IndexVariable];
					if (index == 0)
					{
						const KaraVariable& variable = karaBlob.m_Variables[opRhs.m_Expression.m_IndexVariable - 1];

						PCVariable cvar;

						cvar.m_Size = sizeMap[variable.m_Size];
						cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

						blob.m_Variables.push_back(cvar);

						index = blob.m_Variables.size();
					}

					rhs.m_Type = PCExpression::Type::Variable;
					rhs.m_Variable = index;
				}

				if (opRhs.m_Expression.m_Multiplier != 1)
				{
					std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(rhs));
					std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

					expRhs->m_Type = PCExpression::Type::Literal;
					expRhs->m_Literal = opRhs.m_Expression.m_Multiplier;

					rhs.m_Type = PCExpression::Type::Operator;
					rhs.m_Operator.m_Expression = PCExpression::Expression::Mul;

					rhs.m_Operator.m_Left = std::move(expLhs);
					rhs.m_Operator.m_Right = std::move(expRhs);
				}

				if (opRhs.m_Expression.m_BaseVariable != 0)
				{
					uint32_t& index = variableMap[opRhs.m_Expression.m_BaseVariable];
					if (index == 0)
					{
						const KaraVariable& variable = karaBlob.m_Variables[opRhs.m_Expression.m_BaseVariable - 1];

						PCVariable cvar;

						cvar.m_Size = sizeMap[variable.m_Size];
						cvar.m_Type = typeMap[static_cast<uint8_t>(variable.m_Type)];

						blob.m_Variables.push_back(cvar);

						index = blob.m_Variables.size();
					}

					if (rhs.m_Type == PCExpression::Type::None)
					{
						rhs.m_Type = PCExpression::Type::Variable;
						rhs.m_Variable = index - 1;
					}
					else
					{
						std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(rhs));
						std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

						expRhs->m_Type = PCExpression::Type::Variable;
						expRhs->m_Variable = index - 1;

						rhs.m_Type = PCExpression::Type::Operator;
						rhs.m_Operator.m_Expression = PCExpression::Expression::Add;

						rhs.m_Operator.m_Left = std::move(expLhs);
						rhs.m_Operator.m_Right = std::move(expRhs);
					}
				}

				if (opRhs.m_Expression.m_Offset != 0)
				{
					std::unique_ptr<PCExpression> expLhs = std::make_unique<PCExpression>(std::move(rhs));
					std::unique_ptr<PCExpression> expRhs = std::make_unique<PCExpression>();

					expRhs->m_Type = PCExpression::Type::Literal;
					expRhs->m_Variable = opRhs.m_Expression.m_Offset;

					rhs.m_Type = PCExpression::Type::Operator;
					rhs.m_Operator.m_Expression = PCExpression::Expression::Add;

					rhs.m_Operator.m_Left = std::move(expLhs);
					rhs.m_Operator.m_Right = std::move(expRhs);
				}
			} break;
			}
		} break;
		case KaraInstruction::Type::Addition:
		{
		} break;
		case KaraInstruction::Type::Compare:
		{
		} break;
		case KaraInstruction::Type::AndCompare:
		{
		} break;
		case KaraInstruction::Type::Xor:
		{
		} break;
		case KaraInstruction::Type::Or:
		{
		} break;
		case KaraInstruction::Type::And:
		{
		} break;
		}
	}
}