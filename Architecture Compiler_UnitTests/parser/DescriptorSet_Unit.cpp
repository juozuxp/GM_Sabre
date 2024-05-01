#include "pch.h"
#include "utility/Setup.hpp"
#include <parser/DescriptorSet.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(DescriptorSet_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		std::string_view text = 
		{
			"0F AB -> BTS r/m32|16|64, r32|16|64\n"
			"\n"
			"\n"
			"\n"
			"\n"
			"FF /3 -> CALL m16:32|16|64\n"
			"0F 01 CA -> CLAC"
		};

		DescriptorSet set = DescriptorSet(TokenSet(text));

		std::vector<Descriptor> descriptors = std::vector(set.begin(), set.end());

		Assert::AreEqual<size_t>(descriptors.size(), 3);
	}
};