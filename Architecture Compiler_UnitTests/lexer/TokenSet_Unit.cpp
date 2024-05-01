#include "pch.h"
#include <CppUnitTest.h>
#include <lexer/TokenSet.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(TokenSet_Unit)
{
public:
	TEST_METHOD(Construct_File)
	{
		TokenSet set = TokenSet(std::ifstream("Instructions.asm64"));

		Assert::AreEqual<size_t>(std::vector<Token>(set.begin(), set.end()).size(), 2);
	}

	TEST_METHOD(Construct_Text)
	{
		std::string_view text = 
		{
			"0F 43 -> CMOVAE r32|16|64, r/m32|16|64\n"
			"\n"
			"\n"
			"48 0F C7 /1 -> CMPXCHG16B m128\n"
			"3A -> CMP r8, r/m8\n"
			"\n"
			"0F 48 -> CMOVS r32|16|64, r/m32|16|64\n"
		};

		TokenSet set = TokenSet(text);

		Assert::AreEqual<size_t>(std::vector<Token>(set.begin(), set.end()).size(), 4);
	}

	TEST_METHOD(Insert)
	{
		std::string_view first =
		{
			"0F 43 -> CMOVAE r32|16|64, r/m32|16|64\n"
			"\n"
			"0F 48 -> CMOVS r32|16|64, r/m32|16|64\n"
		};

		std::string_view second =
		{
			"0F 43 -> CMOVAE r32|16|64, r/m32|16|64\n"
			"3A -> CMP r8, r/m8\n"
			"0F 48 -> CMOVS r32|16|64, r/m32|16|64\n"
		};

		TokenSet stream = TokenSet(first);

		stream << TokenSet(second);

		Assert::AreEqual<size_t>(std::vector<Token>(stream.begin(), stream.end()).size(), 5);

		TokenSet add = TokenSet(first);

		add += TokenSet(second);

		Assert::AreEqual<size_t>(std::vector<Token>(add.begin(), add.end()).size(), 5);
	}
};