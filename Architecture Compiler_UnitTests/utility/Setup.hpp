#pragma once
#include <CppUnitTest.h>
#include <string>

#include <parser/DescriptorOperand.hpp>

#include <compiler/BytePackage.hpp>
#include <compiler/Redirection.hpp>
#include <compiler/ByteEntry.hpp>
#include <compiler/Operand.hpp>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<DescriptorOperand::Type>(const DescriptorOperand::Type& value)
			{
				static std::wstring string[] =
				{
					L"none",
					L"reg",
					L"mem",
					L"modrm",
					L"imm",
					L"rel",
					L"moffs"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<DescriptorOperand::Register>(const DescriptorOperand::Register& value)
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

			template<> static std::wstring ToString<DescriptorOperand::Size>(const DescriptorOperand::Size& value)
			{
				static std::wstring string[] =
				{
					L"undefined",
					L"far_32",
					L"base_8",
					L"base_16",
					L"base_32",
					L"base_64",
					L"base_80",
					L"base_128",
					L"base_256",
					L"base_512"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<Redirection::Prefix>(const Redirection::Prefix& value)
			{
				static std::wstring string[] =
				{
					L"Wait",
					L"Repe",
					L"Repne",
					L"x66",
					L"RexW",
					L"Default",
					L"ArrayMAX"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<Operand::Type>(const Operand::Type& value)
			{
				static std::wstring string[] =
				{
					L"none",
					L"reg",
					L"modrm",
					L"imm",
					L"rel",
					L"moffs"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<ByteEntry::PackageType>(const ByteEntry::PackageType& value)
			{
				static std::wstring string[] =
				{
					L"Invalid",
					L"Prefix",
					L"Redirection",
					L"Instruction",
					L"FullRedirection"
				};

				return string[static_cast<uint8_t>(value)];
			}

			template<> static std::wstring ToString<Redirection::Type>(const Redirection::Type& value)
			{
				static std::wstring string[] =
				{
					L"None",
					L"X0F383A",
					L"Prefix",
					L"Mod",
					L"Reg",
					L"Mem"
				};

				return string[static_cast<uint8_t>(value)];
			}
		}
	}
}