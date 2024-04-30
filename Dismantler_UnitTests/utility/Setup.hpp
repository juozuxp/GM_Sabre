#pragma once
#include <CppUnitTest.h>
#include <string>

#include <explorer/ExecutableView.hpp>
#include <pseudoc/PCExpression.hpp>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<ExecutableViewSection::Type>(const ExecutableViewSection::Type& value)
			{
				static std::wstring string[] =
				{
					L"Bytes",
					L"Code"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<PCExpression::Type>(const PCExpression::Type& value)
			{
				static std::wstring string[] =
				{
					L"None",
					L"Literal",
					L"Variable",
					L"Operation",
					L"Dereference",
					L"Reference"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<PCExpression::Expression>(const PCExpression::Expression& value)
			{
				static std::wstring string[] =
				{
					L"Add",
					L"Sub",
					L"Mul",
					L"Xor",
					L"Or",
					L"And",
					L"Not"
				};

				return string[static_cast<uint8_t>(value)];
			}
		}
	}
}