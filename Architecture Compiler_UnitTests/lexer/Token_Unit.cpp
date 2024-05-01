#include "pch.h"
#include <CppUnitTest.h>
#include <lexer/Token.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Token_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		Token token = Token(" 0F BA /4->BT r/m32|16 |64, imm8");

		Assert::AreEqual(token.GetName().c_str(), "bt", true);

		Assert::AreEqual<size_t>(token.GetBytes().size(), 3);
		Assert::AreEqual(token.GetBytes()[0].c_str(), "0F", true);
		Assert::AreEqual(token.GetBytes()[1].c_str(), "BA", true);
		Assert::AreEqual(token.GetBytes()[2].c_str(), "/4", true);

		Assert::AreEqual<size_t>(token.GetOperands().size(), 2);

		Assert::AreEqual<size_t>(token.GetOperands()[0].size(), 3);
		Assert::AreEqual(token.GetOperands()[0][0].c_str(), "r/m32", true);
		Assert::AreEqual(token.GetOperands()[0][1].c_str(), "16", true);
		Assert::AreEqual(token.GetOperands()[0][2].c_str(), "64", true);

		Assert::AreEqual<size_t>(token.GetOperands()[1].size(), 1);
		Assert::AreEqual(token.GetOperands()[1][0].c_str(), "imm8", true);

		token = Token("0F 40 -> CMOVO r32|16|64, r/m32|16|64,");

		Assert::AreEqual(token.GetName().c_str(), "cmovo", true);

		Assert::AreEqual<size_t>(token.GetBytes().size(), 2);
		Assert::AreEqual(token.GetBytes()[0].c_str(), "0F", true);
		Assert::AreEqual(token.GetBytes()[1].c_str(), "40", true);

		Assert::AreEqual<size_t>(token.GetOperands().size(), 2);

		Assert::AreEqual<size_t>(token.GetOperands()[0].size(), 3);
		Assert::AreEqual(token.GetOperands()[0][0].c_str(), "r32", true);
		Assert::AreEqual(token.GetOperands()[0][1].c_str(), "16", true);
		Assert::AreEqual(token.GetOperands()[0][2].c_str(), "64", true);

		Assert::AreEqual<size_t>(token.GetOperands()[1].size(), 3);
		Assert::AreEqual(token.GetOperands()[1][0].c_str(), "r/m32", true);
		Assert::AreEqual(token.GetOperands()[1][1].c_str(), "16", true);
		Assert::AreEqual(token.GetOperands()[1][2].c_str(), "64", true);

		token = Token("0F 01 CA -> CLAC");

		Assert::IsTrue(token.GetOperands().empty());

		Assert::AreEqual(token.GetName().c_str(), "clac", true);

		Assert::AreEqual<size_t>(token.GetBytes().size(), 3);
		Assert::AreEqual(token.GetBytes()[0].c_str(), "0F", true);
		Assert::AreEqual(token.GetBytes()[1].c_str(), "01", true);
		Assert::AreEqual(token.GetBytes()[2].c_str(), "CA", true);

		token = Token("0F 40 ->");

		Assert::IsTrue(token.GetName().empty());
		Assert::IsTrue(token.GetBytes().empty());
		Assert::IsTrue(token.GetOperands().empty());

		token = Token("");

		Assert::IsTrue(token.GetName().empty());
		Assert::IsTrue(token.GetBytes().empty());
		Assert::IsTrue(token.GetOperands().empty());

		token = Token("0F 40");

		Assert::IsTrue(token.GetName().empty());
		Assert::IsTrue(token.GetBytes().empty());
		Assert::IsTrue(token.GetOperands().empty());
	}
};