#include "pch.h"
#include "utility/Setup.hpp"
#include <compiler/BytePackage.hpp>
#include <compiler/InstructionSet.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(InstructionSet_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		std::string_view text = 
		{
			"B5 -> MOV CH|REX, imm8\n"
			"B6 -> MOV DH|REX, imm8\n"
			"B7 -> MOV BH|REX, imm8\n"
			"B8 -> MOV EAX|AX|RAX|REX, imm32|16|64\n"
			"B9 -> MOV ECX|CX|RCX|REX, imm32|16|64\n"
			"BA -> MOV EDX|DX|RDX|REX, imm32|16|64\n"
			"66 0F 38 3C -> PMAXSB xmm, xmm/m128\n"
			"66 0F 38 3D -> PMAXSD xmm, xmm/m128\n"
			"66 0F 38 3E -> PMAXUW xmm, xmm/m128\n"
			"66 0F 38 3F -> PMAXUD xmm, xmm/m128\n"
			"9B -> FWAIT\n"
			"0F C7 /6 -> VMPTRLD m64\n"
			"0F C7 /7 -> VMPTRST m64\n"
			"F3 0F C7 /6 -> VMXON m64\n"
			"66 0F C7 /6 -> VMCLEAR m64\n"
			"D9 F7 -> FINCSTP\n"
			"D9 /0 -> FLD m32\n"
			"DD /0 -> FLD m64\n"
			"DB /5 -> FLD m80\n"
			"D9 /0 -> FLD ST\n"
			"D9 E8 -> FLD1\n"
			"D9 E9 -> FLDL2T\n"
			"D9 EA -> FLDL2E\n"
			"D9 EB -> FLDPI\n"
			"D9 EC -> FLDLG2\n"
			"D9 ED -> FLDLN2\n"
			"D9 EE -> FLDZ\n"
			"66 0F 3A 09 -> ROUNDPD xmm, xmm/m128, imm8\n"
			"66 0F 3A 08 -> ROUNDPS xmm, xmm/m128, imm8\n"
			"66 0F 3A 0B -> ROUNDSD xmm, xmm/m64, imm8\n"
			"66 0F 3A 0A -> ROUNDSS xmm, xmm/m32, imm8\n"
		};

		const char* textTypes[] = 
		{
			"INVALID",
			"MOV",
			"PMAXSB",
			"PMAXSD",
			"PMAXUW",
			"PMAXUD",
			"FWAIT",
			"VMPTRLD",
			"VMPTRST",
			"VMXON",
			"VMCLEAR",
			"FINCSTP",
			"FLD",
			"FLD1",
			"FLDL2T",
			"FLDL2E",
			"FLDPI",
			"FLDLG2",
			"FLDLN2",
			"FLDZ",
			"ROUNDPD",
			"ROUNDPS",
			"ROUNDSD",
			"ROUNDSS"
		};

		TokenSet tokens = TokenSet(text);
		DescriptorSet descriptors = DescriptorSet(tokens);
		InstructionSet instructions = InstructionSet(descriptors);

		std::vector<std::string> types = instructions.GetTypes();

		for (const std::string& type : types)
		{
			bool present = false;
			for (const char* textType : textTypes)
			{
				if (_stricmp(type.c_str(), textType) == 0)
				{
					present = true;
					break;
				}
			}

			Assert::IsTrue(present);
		}

		BytePackage package = instructions.GetPackage();

		Assert::AreEqual<uint32_t>(package.GetCount(), 583);
		Assert::AreEqual<size_t>(package.GetRaw().size(), 582);
	}
};