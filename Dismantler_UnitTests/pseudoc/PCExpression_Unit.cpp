#include "pch.h"
#include <CppUnitTest.h>
#include "utility/Setup.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(PCExpression_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PCExpression first;
		PCExpression second;

		memset(&second, 0, sizeof(second));

		Assert::IsTrue(memcmp(&first, &second, sizeof(first)) == 0);
	}

	TEST_METHOD(Move_none_Construct)
	{
		PCExpression none;

		Assert::AreEqual(none.m_Type, PCExpression::Type::None);

		PCExpression noneConstruct = PCExpression(std::move(none));

		Assert::AreEqual(noneConstruct.m_Type, PCExpression::Type::None);
	}

	TEST_METHOD(Move_none_Equal)
	{
		PCExpression none;

		Assert::AreEqual(none.m_Type, PCExpression::Type::None);

		PCExpression noneEqual;

		noneEqual = std::move(none);

		Assert::AreEqual(noneEqual.m_Type, PCExpression::Type::None);
	}

	TEST_METHOD(Move_literal_Construct)
	{
		PCExpression literal;

		literal.m_Type = PCExpression::Type::Literal;
		literal.m_Literal = 0x1234;

		PCExpression literalConstruct = PCExpression(std::move(literal));

		Assert::AreEqual(literalConstruct.m_Type, PCExpression::Type::Literal);
		Assert::AreEqual<uint64_t>(literalConstruct.m_Literal, 0x1234);
	}

	TEST_METHOD(Move_literal_Equal)
	{
		PCExpression literal;

		literal.m_Type = PCExpression::Type::Literal;
		literal.m_Literal = 0x1234;

		PCExpression literalEqual;

		literalEqual = std::move(literal);

		Assert::AreEqual(literalEqual.m_Type, PCExpression::Type::Literal);
		Assert::AreEqual<uint64_t>(literalEqual.m_Literal, 0x1234);
	}

	TEST_METHOD(Move_variable_Construct)
	{
		PCExpression variable;

		variable.m_Type = PCExpression::Type::Variable;
		variable.m_Variable = 0x10;

		PCExpression variableConstruct = PCExpression(std::move(variable));

		Assert::AreEqual(variableConstruct.m_Type, PCExpression::Type::Variable);
		Assert::AreEqual<uint32_t>(variableConstruct.m_Variable, 0x10);
	}

	TEST_METHOD(Move_variable_Equal)
	{
		PCExpression variable;

		variable.m_Type = PCExpression::Type::Variable;
		variable.m_Variable = 0x10;

		PCExpression variableEqual;

		variableEqual = std::move(variable);

		Assert::AreEqual(variableEqual.m_Type, PCExpression::Type::Variable);
		Assert::AreEqual<uint32_t>(variableEqual.m_Variable, 0x10);
	}

	TEST_METHOD(Move_operation_Construct)
	{
		PCExpression operation;

		operation.m_Type = PCExpression::Type::Operation;
		operation.m_Operation.m_Expression = PCExpression::Expression::Add;

		operation.m_Operation.m_Left = std::make_unique<PCExpression>();
		operation.m_Operation.m_Right = std::make_unique<PCExpression>();

		PCExpression* left = operation.m_Operation.m_Left.get();
		PCExpression* right = operation.m_Operation.m_Right.get();

		PCExpression operationConstruct = PCExpression(std::move(operation));

		Assert::AreEqual<void*>(operation.m_Operation.m_Left.get(), nullptr);
		Assert::AreEqual<void*>(operation.m_Operation.m_Right.get(), nullptr);

		Assert::AreEqual(operationConstruct.m_Type, PCExpression::Type::Operation);
		Assert::AreEqual(operationConstruct.m_Operation.m_Expression, PCExpression::Expression::Add);

		Assert::AreEqual<void*>(operationConstruct.m_Operation.m_Left.get(), left);
		Assert::AreEqual<void*>(operationConstruct.m_Operation.m_Right.get(), right);
	}

	TEST_METHOD(Move_operation_Equal)
	{
		PCExpression operation;

		operation.m_Type = PCExpression::Type::Operation;
		operation.m_Operation.m_Expression = PCExpression::Expression::Add;

		operation.m_Operation.m_Left = std::make_unique<PCExpression>();
		operation.m_Operation.m_Right = std::make_unique<PCExpression>();

		PCExpression* left = operation.m_Operation.m_Left.get();
		PCExpression* right = operation.m_Operation.m_Right.get();

		PCExpression operationEqual;

		operationEqual = std::move(operation);

		Assert::AreEqual<void*>(operation.m_Operation.m_Left.get(), nullptr);
		Assert::AreEqual<void*>(operation.m_Operation.m_Right.get(), nullptr);

		Assert::AreEqual(operationEqual.m_Type, PCExpression::Type::Operation);
		Assert::AreEqual(operationEqual.m_Operation.m_Expression, PCExpression::Expression::Add);

		Assert::AreEqual<void*>(operationEqual.m_Operation.m_Left.get(), left);
		Assert::AreEqual<void*>(operationEqual.m_Operation.m_Right.get(), right);
	}

	TEST_METHOD(Move_dereference_Construct)
	{
		PCExpression dereference;

		dereference.m_Type = PCExpression::Type::Dereference;
		dereference.m_Dereference = std::make_unique<PCExpression>();

		PCExpression* expression = dereference.m_Dereference.get();

		PCExpression dereferenceConstruct = PCExpression(std::move(dereference));

		Assert::AreEqual<void*>(dereference.m_Dereference.get(), nullptr);

		Assert::AreEqual(dereferenceConstruct.m_Type, PCExpression::Type::Dereference);
		Assert::AreEqual<void*>(dereferenceConstruct.m_Dereference.get(), expression);
	}

	TEST_METHOD(Move_dereference_Equal)
	{
		PCExpression dereference;

		dereference.m_Type = PCExpression::Type::Dereference;
		dereference.m_Dereference = std::make_unique<PCExpression>();

		PCExpression* expression = dereference.m_Dereference.get();

		PCExpression dereferenceEqual;

		dereferenceEqual = std::move(dereference);

		Assert::AreEqual<void*>(dereference.m_Dereference.get(), nullptr);

		Assert::AreEqual(dereferenceEqual.m_Type, PCExpression::Type::Dereference);
		Assert::AreEqual<void*>(dereferenceEqual.m_Dereference.get(), expression);
	}

	TEST_METHOD(Move_reference_Construct)
	{
		PCExpression reference;

		reference.m_Type = PCExpression::Type::Reference;
		reference.m_Variable = 0x15;

		PCExpression referenceConstruct = PCExpression(std::move(reference));

		Assert::AreEqual(referenceConstruct.m_Type, PCExpression::Type::Reference);
		Assert::AreEqual<uint32_t>(referenceConstruct.m_Variable, 0x15);
	}

	TEST_METHOD(Move_reference_Equal)
	{
		PCExpression reference;

		reference.m_Type = PCExpression::Type::Reference;
		reference.m_Variable = 0x15;

		PCExpression referenceEqual;

		referenceEqual = std::move(reference);

		Assert::AreEqual(referenceEqual.m_Type, PCExpression::Type::Reference);
		Assert::AreEqual<uint32_t>(referenceEqual.m_Variable, 0x15);
	}

	TEST_METHOD(Copy_none_Construct)
	{
		PCExpression none;

		Assert::AreEqual(none.m_Type, PCExpression::Type::None);

		PCExpression noneConstruct = PCExpression(none);

		Assert::AreEqual(noneConstruct.m_Type, PCExpression::Type::None);
	}

	TEST_METHOD(Copy_none_Equal)
	{
		PCExpression none;

		Assert::AreEqual(none.m_Type, PCExpression::Type::None);

		PCExpression noneEqual;

		noneEqual = none;

		Assert::AreEqual(noneEqual.m_Type, PCExpression::Type::None);
	}

	TEST_METHOD(Copy_literal_Construct)
	{
		PCExpression literal;

		literal.m_Type = PCExpression::Type::Literal;
		literal.m_Literal = 0x1234;

		PCExpression literalConstruct = PCExpression(literal);

		Assert::AreEqual(literalConstruct.m_Type, PCExpression::Type::Literal);
		Assert::AreEqual<uint64_t>(literalConstruct.m_Literal, 0x1234);
	}

	TEST_METHOD(Copy_literal_Equal)
	{
		PCExpression literal;

		literal.m_Type = PCExpression::Type::Literal;
		literal.m_Literal = 0x1234;

		PCExpression literalEqual;

		literalEqual = literal;

		Assert::AreEqual(literalEqual.m_Type, PCExpression::Type::Literal);
		Assert::AreEqual<uint64_t>(literalEqual.m_Literal, 0x1234);
	}

	TEST_METHOD(Copy_variable_Construct)
	{
		PCExpression variable;

		variable.m_Type = PCExpression::Type::Variable;
		variable.m_Variable = 0x10;

		PCExpression variableConstruct = PCExpression(variable);

		Assert::AreEqual(variableConstruct.m_Type, PCExpression::Type::Variable);
		Assert::AreEqual<uint32_t>(variableConstruct.m_Variable, 0x10);
	}

	TEST_METHOD(Copy_variable_Equal)
	{
		PCExpression variable;

		variable.m_Type = PCExpression::Type::Variable;
		variable.m_Variable = 0x10;

		PCExpression variableEqual;

		variableEqual = variable;

		Assert::AreEqual(variableEqual.m_Type, PCExpression::Type::Variable);
		Assert::AreEqual<uint32_t>(variableEqual.m_Variable, 0x10);
	}

	TEST_METHOD(Copy_operation_Construct)
	{
		PCExpression operation;

		operation.m_Type = PCExpression::Type::Operation;
		operation.m_Operation.m_Expression = PCExpression::Expression::Add;

		operation.m_Operation.m_Left = std::make_unique<PCExpression>();
		operation.m_Operation.m_Right = std::make_unique<PCExpression>();

		operation.m_Operation.m_Left->m_Type = PCExpression::Type::Variable;
		operation.m_Operation.m_Left->m_Variable = 0x100;

		operation.m_Operation.m_Right->m_Type = PCExpression::Type::Literal;
		operation.m_Operation.m_Right->m_Literal = 0xC0FFEE;

		PCExpression operationConstruct = PCExpression(operation);

		Assert::AreEqual(operationConstruct.m_Type, PCExpression::Type::Operation);
		Assert::AreEqual(operationConstruct.m_Operation.m_Expression, PCExpression::Expression::Add);

		Assert::AreEqual(operationConstruct.m_Operation.m_Left->m_Type, PCExpression::Type::Variable);
		Assert::AreEqual<uint32_t>(operationConstruct.m_Operation.m_Left->m_Variable, 0x100);

		Assert::AreEqual(operationConstruct.m_Operation.m_Right->m_Type, PCExpression::Type::Literal);
		Assert::AreEqual<uint64_t>(operationConstruct.m_Operation.m_Right->m_Variable, 0xC0FFEE);
	}

	TEST_METHOD(Copy_operation_Equal)
	{
		PCExpression operation;

		operation.m_Type = PCExpression::Type::Operation;
		operation.m_Operation.m_Expression = PCExpression::Expression::Add;

		operation.m_Operation.m_Left = std::make_unique<PCExpression>();
		operation.m_Operation.m_Right = std::make_unique<PCExpression>();

		operation.m_Operation.m_Left->m_Type = PCExpression::Type::Variable;
		operation.m_Operation.m_Left->m_Variable = 0x100;

		operation.m_Operation.m_Right->m_Type = PCExpression::Type::Literal;
		operation.m_Operation.m_Right->m_Literal = 0xC0FFEE;

		PCExpression operationEqual;

		operationEqual = operation;

		Assert::AreEqual(operationEqual.m_Type, PCExpression::Type::Operation);
		Assert::AreEqual(operationEqual.m_Operation.m_Expression, PCExpression::Expression::Add);

		Assert::AreEqual(operationEqual.m_Operation.m_Left->m_Type, PCExpression::Type::Variable);
		Assert::AreEqual<uint32_t>(operationEqual.m_Operation.m_Left->m_Variable, 0x100);

		Assert::AreEqual(operationEqual.m_Operation.m_Right->m_Type, PCExpression::Type::Literal);
		Assert::AreEqual<uint64_t>(operationEqual.m_Operation.m_Right->m_Variable, 0xC0FFEE);
	}

	TEST_METHOD(Copy_dereference_Construct)
	{
		PCExpression dereference;

		dereference.m_Type = PCExpression::Type::Dereference;
		dereference.m_Dereference = std::make_unique<PCExpression>();

		dereference.m_Dereference->m_Type = PCExpression::Type::Reference;
		dereference.m_Dereference->m_Variable = 0x40;

		PCExpression dereferenceConstruct = PCExpression(dereference);

		Assert::AreEqual(dereferenceConstruct.m_Type, PCExpression::Type::Dereference);

		Assert::AreEqual(dereferenceConstruct.m_Dereference->m_Type, PCExpression::Type::Reference);
		Assert::AreEqual<uint32_t>(dereferenceConstruct.m_Dereference->m_Variable, 0x40);
	}

	TEST_METHOD(Copy_dereference_Equal)
	{
		PCExpression dereference;

		dereference.m_Type = PCExpression::Type::Dereference;
		dereference.m_Dereference = std::make_unique<PCExpression>();

		dereference.m_Dereference->m_Type = PCExpression::Type::Reference;
		dereference.m_Dereference->m_Variable = 0x40;

		PCExpression dereferenceEqual;

		dereferenceEqual = dereference;

		Assert::AreEqual(dereferenceEqual.m_Type, PCExpression::Type::Dereference);

		Assert::AreEqual(dereferenceEqual.m_Dereference->m_Type, PCExpression::Type::Reference);
		Assert::AreEqual<uint32_t>(dereferenceEqual.m_Dereference->m_Variable, 0x40);
	}

	TEST_METHOD(Copy_reference_Construct)
	{
		PCExpression reference;

		reference.m_Type = PCExpression::Type::Reference;
		reference.m_Variable = 0x15;

		PCExpression referenceConstruct = PCExpression(reference);

		Assert::AreEqual(referenceConstruct.m_Type, PCExpression::Type::Reference);
		Assert::AreEqual<uint32_t>(referenceConstruct.m_Variable, 0x15);
	}

	TEST_METHOD(Copy_reference_Equal)
	{
		PCExpression reference;

		reference.m_Type = PCExpression::Type::Reference;
		reference.m_Variable = 0x15;

		PCExpression referenceEqual;

		referenceEqual = reference;

		Assert::AreEqual(referenceEqual.m_Type, PCExpression::Type::Reference);
		Assert::AreEqual<uint32_t>(referenceEqual.m_Variable, 0x15);
	}
};