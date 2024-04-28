#pragma once
#include <CppUnitTest.h>
#include <string>

#include <include/Disassembler.hpp>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<ILMemoryScaler>(const ILMemoryScaler& value)
			{
				static std::wstring string[] =
				{ 
					L"ILMemoryScaler_1",
					L"ILMemoryScaler_2",
					L"ILMemoryScaler_4",
					L"ILMemoryScaler_8"
				};

				return string[value];
			}

			template<> static std::wstring ToString<ILOperandScale>(const ILOperandScale& value)
			{
				static std::wstring string[] =
				{
					L"ILOperandScale_Undefined",
					L"ILOperandScale_8",
					L"ILOperandScale_16",
					L"ILOperandScale_32",
					L"ILOperandScale_64",
					L"ILOperandScale_80",
					L"ILOperandScale_128",
					L"ILOperandScale_256",
					L"ILOperandScale_512"
				};

				return string[value];
			}
			
			template<> static std::wstring ToString<ILOperandType>(const ILOperandType& value)
			{
				static std::wstring string[] =
				{
					L"ILOperandType_None",
					L"ILOperandType_Memory",
					L"ILOperandType_Register",
					L"ILOperandType_Value",
					L"ILOperandType_ValueRelative",
					L"ILOperandType_MemoryRelative",
					L"ILOperandType_MemoryAbsolute"
				};

				return string[value];
			}

			template<> static std::wstring ToString<InsType>(const InsType& value)
			{
				const char* name = TypeToName[value];

				size_t length = strlen(name);
				std::wstring wide = std::wstring(length + 1, L'\0');

				size_t converted;

				mbstowcs_s(&converted, wide.data(), wide.size(), name, length);

				wide.erase(wide.size() - 1);
				return wide;
			}

			template<> static std::wstring ToString<Register>(const Register& value)
			{
				static std::wstring string[] =
				{
					L"general",
					L"xmm",
					L"mm",
					L"bnd",
					L"st",
					L"sreg",
					L"cr",
					L"dr"
				};

				return string[static_cast<uint8_t>(value)];
			}
		}
	}
}