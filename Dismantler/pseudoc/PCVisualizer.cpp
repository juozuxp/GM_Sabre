#include "PCVisualizer.hpp"
#include <sstream>
#include <format>
#include <unordered_map>

void PCVisualizer::ToConsole(const PCBlob& blob)
{
	wprintf(ToString(blob).c_str());
}

std::wstring PCVisualizer::ToString(const PCBlob& blob)
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

	std::vector<std::wstring> varNames;
	std::vector<uint32_t> arguments;
	
	uint32_t variableIndex = 0;
	for (uint32_t i = 0; i < blob.m_Variables.size(); i++)
	{
		const PCVariable& variable = blob.m_Variables[i];
		switch (variable.m_Type)
		{
		case PCVariable::Type::None:
		{
			varNames.push_back(L"");
		} break;
		case PCVariable::Type::Local:
		{
			varNames.push_back(std::format(L"v{:}", variableIndex++));
		} break;
		case PCVariable::Type::Static:
		{
			varNames.push_back(std::format(L"s_{:#016}", variable.m_Static));
		} break;
		case PCVariable::Type::Argument:
		{
			varNames.push_back(std::format(L"a{:}", variable.m_Argument));

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
		stream << varNames[argument - 1];
		stream << L", ";
	}

	stream.seekp(-2, std::ios_base::cur);
	stream << L")\n{\n";

	std::unordered_map<std::shared_ptr<PCLine>, std::wstring> labelNames;

	uint32_t labelIndex = 0;
	for (const std::shared_ptr<PCLine>& label : blob.m_Labels)
	{
		labelNames[label] = std::format(L"Label_{:}", labelIndex++);
	}

	uint32_t indent = 1;
	for (const std::shared_ptr<PCLine>& line : blob.m_Lines)
	{
		stream << std::wstring(indent, ' ');

		switch (line->m_Type)
		{
		case PCLine::Type::Assign:
		{

		} break;
		}

		stream << '\n';
	}

	return stream.str();
}