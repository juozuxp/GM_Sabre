#include "PCVisualizer.hpp"
#include <sstream>
#include <format>

PCVisualizer::PCVisualizer(const PEBuffer& buffer)
{
	m_Buffer = &buffer;
}

void PCVisualizer::ToConsole(const PCBlob& blob) const
{
	wprintf(ToString(blob).c_str());
}

std::wstring PCVisualizer::ToString(const PCBlob& blob) const
{
	constexpr std::wstring_view sizeToString[] = { L"int8_t", L"int16_t", L"int32_t", L"int64_t" };

	std::wstringstream stream;

	uint32_t returnVariable = 0;
	for (const std::shared_ptr<PCLine>& line : blob.m_Lines)
	{
		if (line->m_Type == PCLine::Type::Return)
		{
			if (line->m_Return.m_Result.m_Type != PCExpression::Type::Variable)
			{
				continue;
			}

			returnVariable = line->m_Return.m_Result.m_Variable + 1;
		}
	}

	if (returnVariable != 0)
	{
		stream << sizeToString[static_cast<uint32_t>(blob.m_Variables[returnVariable - 1].m_Size)];
	}
	else
	{
		stream << L"void";
	}

	stream << std::format(L" f_{:016X}(", blob.m_Function);

	std::vector<uint32_t> arguments;
	State state;

	state.m_Blob = &blob;
	
	uint32_t variableIndex = 0;
	for (uint32_t i = 0; i < blob.m_Variables.size(); i++)
	{
		const PCVariable& variable = blob.m_Variables[i];
		switch (variable.m_Type)
		{
		case PCVariable::Type::None:
		{
			state.m_VariableNames.push_back(L"");
		} break;
		case PCVariable::Type::Local:
		{
			state.m_VariableNames.push_back(std::format(L"v{:}", variableIndex++));
		} break;
		case PCVariable::Type::Static:
		{
			state.m_VariableNames.push_back(std::format(L"s_{:016X}", variable.m_Static));
		} break;
		case PCVariable::Type::Argument:
		{
			state.m_VariableNames.push_back(std::format(L"a{:}", variable.m_Argument));

			if (arguments.size() < (variable.m_Argument + 1))
			{
				arguments.resize(variable.m_Argument + 1);
			}

			arguments[variable.m_Argument] = i + 1;
		} break;
		}
	}

	for (uint32_t argument : arguments)
	{
		if (argument == 0)
		{
			break;
		}

		stream << sizeToString[static_cast<uint32_t>(blob.m_Variables[argument - 1].m_Size)];
		stream << L' ';
		stream << state.m_VariableNames[argument - 1];
		stream << L", ";
	}

	if (arguments.size() != 0)
	{
		stream.seekp(-2, std::ios_base::cur);
	}

	stream << L")\n{\n";

	for (uint32_t i = 0; i < blob.m_Variables.size(); i++)
	{
		const PCVariable& variable = blob.m_Variables[i];

		if (variable.m_Type != PCVariable::Type::Local)
		{
			continue;
		}

		stream << L'\t' << sizeToString[static_cast<uint32_t>(blob.m_Variables[i].m_Size)] << L' ' << state.m_VariableNames[i] << L";\n";
	}

	stream << L"\n\n";

	uint32_t labelIndex = 0;
	for (const std::shared_ptr<PCLine>& label : blob.m_Labels)
	{
		state.m_LabelNames[label] = std::format(L"Label_{:}", labelIndex++);
	}

	uint32_t scopeIndex = 1;
	std::unordered_map<std::shared_ptr<PCLine>, uint32_t> scope;
	for (const std::shared_ptr<PCLine>& line : blob.m_Lines)
	{
		const auto& closing = scope.find(line);
		if (closing != scope.end())
		{
			for (uint32_t i = 0; i < closing->second; i++, scopeIndex--)
			{
				stream << std::wstring(scopeIndex - 1, '\t') << L"}\n";
			}

			scope.erase(line);
		}

		const auto& label = state.m_LabelNames.find(line);
		if (label != state.m_LabelNames.end())
		{
			stream << label->second << L":\n";
		}

		stream << std::wstring(scopeIndex, '\t');

		switch (line->m_Type)
		{
		case PCLine::Type::Assign:
		{
			stream << ExpressionToString(state, line->m_Assign.m_Left) << L" = " << ExpressionToString(state, line->m_Assign.m_Right) << L';';
		} break;
		case PCLine::Type::Invoke:
		{
			if (line->m_Invoke.m_Result.m_Type != PCExpression::Type::None)
			{
				stream << std::format(L"{:} = ", ExpressionToString(state, line->m_Invoke.m_Result));
			}

			if (line->m_Invoke.m_Function.m_Type == PCExpression::Type::Literal)
			{
				stream << std::format(L"f_{:016X}(", line->m_Invoke.m_Function.m_Literal);
			}
			else
			{
				stream << std::format(L"{:}(", ExpressionToString(state, line->m_Invoke.m_Function));
			}

			for (const PCExpression& argument : line->m_Invoke.m_Arguments)
			{
				stream << std::format(L"{:}, ", ExpressionToString(state, argument));
			}

			if (line->m_Invoke.m_Arguments.size() != 0)
			{
				stream.seekp(-2, std::ios_base::cur);
			}

			stream << L");";
		} break;
		case PCLine::Type::Equal:
		{
			stream << std::format(L"if ({:} == {:})\n", ExpressionToString(state, line->m_Condition.m_Left), ExpressionToString(state, line->m_Condition.m_Right));
			stream << std::wstring(scopeIndex, '\t') << L'{';

			scopeIndex++;
			scope[line->m_Condition.m_Else]++;
		} break;
		case PCLine::Type::NotEqual:
		{
			stream << std::format(L"if ({:} != {:})\n", ExpressionToString(state, line->m_Condition.m_Left), ExpressionToString(state, line->m_Condition.m_Right));
			stream << std::wstring(scopeIndex, '\t') << L'{';

			scopeIndex++;
			scope[line->m_Condition.m_Else]++;
		} break;
		case PCLine::Type::Less:
		{
			stream << std::format(L"if ({:} < {:})\n", ExpressionToString(state, line->m_Condition.m_Left), ExpressionToString(state, line->m_Condition.m_Right));
			stream << std::wstring(scopeIndex, '\t') << L'{';

			scopeIndex++;
			scope[line->m_Condition.m_Else]++;
		} break;
		case PCLine::Type::Greater:
		{
			stream << std::format(L"if ({:} > {:})\n", ExpressionToString(state, line->m_Condition.m_Left), ExpressionToString(state, line->m_Condition.m_Right));
			stream << std::wstring(scopeIndex, '\t') << L'{';

			scopeIndex++;
			scope[line->m_Condition.m_Else]++;
		} break;
		case PCLine::Type::LessEqual:
		{
			stream << std::format(L"if ({:} <= {:})\n", ExpressionToString(state, line->m_Condition.m_Left), ExpressionToString(state, line->m_Condition.m_Right));
			stream << std::wstring(scopeIndex, '\t') << L'{';

			scopeIndex++;
			scope[line->m_Condition.m_Else]++;
		} break;
		case PCLine::Type::GreaterEqual:
		{
			stream << std::format(L"if ({:} >= {:})\n", ExpressionToString(state, line->m_Condition.m_Left), ExpressionToString(state, line->m_Condition.m_Right));
			stream << std::wstring(scopeIndex, '\t') << L'{';

			scopeIndex++;
			scope[line->m_Condition.m_Else]++;
		} break;
		case PCLine::Type::Goto:
		{
			stream << L"goto " << state.m_LabelNames[line->m_Goto.m_Destination] << L';';
		} break;
		case PCLine::Type::Return:
		{
			if (line->m_Return.m_Result.m_Type != PCExpression::Type::None)
			{
				stream << L"return " << ExpressionToString(state, line->m_Return.m_Result) << L';';
			}
			else
			{
				stream << L"return;";
			}
		} break;
		}

		stream << '\n';
	}

	stream << "}";
	return stream.str();
}

bool PCVisualizer::IsString(uintptr_t address) const
{
	const char* string = reinterpret_cast<const char*>(m_Buffer->GetBuffer()) + (address - m_Buffer->GetOptionalHeader64().ImageBase);

	for (; *string != '\0'; string++)
	{
		if (static_cast<unsigned char>(*string) > 127)
		{
			return false;
		}

		if (!std::isprint(*string))
		{
			return false;
		}
	}

	return true;
}

std::wstring PCVisualizer::ExpressionToString(const State& state, const PCExpression& expression) const
{
	constexpr wchar_t expressionToChar[] = { L'+', L'-', L'*', L'^', L'|', L'&', L'~' };

	switch (expression.m_Type)
	{
	case PCExpression::Type::Variable:
	{
		return state.m_VariableNames[expression.m_Variable];
	} break;
	case PCExpression::Type::Literal:
	{
		return std::format(L"{:}", expression.m_Literal);
	} break;
	case PCExpression::Type::Dereference:
	{
		return std::format(L"*{:}", ExpressionToString(state, *expression.m_Dereference));
	} break;
	case PCExpression::Type::Operation:
	{
		if (expression.m_Operation.m_Expression == PCExpression::Expression::Not)
		{
			return std::format(L"~({:})", ExpressionToString(state, *expression.m_Operation.m_Left));
		}

		return std::format(L"({:} {:} {:})", ExpressionToString(state, *expression.m_Operation.m_Left), expressionToChar[static_cast<uint32_t>(expression.m_Operation.m_Expression)], ExpressionToString(state, *expression.m_Operation.m_Right));
	} break;
	case PCExpression::Type::Reference:
	{
		const PCVariable& variable = state.m_Blob->m_Variables[expression.m_Variable];

		if (variable.m_Type == PCVariable::Type::Static && IsString(variable.m_Static))
		{
			const char* string = reinterpret_cast<const char*>(m_Buffer->GetBuffer()) + (variable.m_Static - m_Buffer->GetOptionalHeader64().ImageBase);
			
			uint32_t length = strlen(string);
			std::wstring wide = std::wstring(length + 1, L'\0');

			size_t converted;

			mbstowcs_s(&converted, wide.data(), wide.size(), string, length);

			wide.erase(wide.size() - 1);
			return std::format(L"\"{:}\"", wide);
		}

		return std::format(L"&{:}", state.m_VariableNames[expression.m_Variable]);
	} break;
	}

	return L"";
}