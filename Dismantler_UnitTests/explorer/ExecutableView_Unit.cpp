#include "pch.h"
#include "utility/Setup.hpp"
#include <explorer/ExecutableView.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ExecutableView_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		PEBuffer pe = PEBuffer(L"kernel32.dll");

		Disassembler dissasembler;
		Visualizer visualizer;

		ExecutableView view = ExecutableView(pe, dissasembler, visualizer);

		Assert::AreEqual<uintptr_t>(view.m_BaseAddress, 0x0000000180000000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Size, 9);

		Assert::AreEqual(view.m_Sections.m_Array[0].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[0].m_Start, 0x00000000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[0].m_Size, 0x00001000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[0].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[1].m_Type, ExecutableViewSection::Type::Code);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[1].m_Start, 0x00001000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[1].m_Size, 0x0007d4cb);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[1].m_Visuals.m_Size, 0x00057597);

		Assert::AreEqual(view.m_Sections.m_Array[2].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[2].m_Start, 0x0007e4cb);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[2].m_Size, 0x00000135);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[2].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[3].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[3].m_Start, 0x0007f000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[3].m_Size, 0x00033000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[3].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[4].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[4].m_Start, 0x000b2000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[4].m_Size, 0x00000600);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[4].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[5].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[5].m_Start, 0x000b4000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[5].m_Size, 0x00005600);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[5].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[6].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[6].m_Start, 0x000ba000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[6].m_Size, 0x00000200);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[6].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[7].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[7].m_Start, 0x000bb000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[7].m_Size, 0x00000600);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[7].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[8].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[8].m_Start, 0x000bc000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[8].m_Size, 0x00000400);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[8].m_Visuals.m_Size, 0);

		pe = PEBuffer(L"kernel32_32bit.dll");

		view = ExecutableView(pe, dissasembler, visualizer);

		Assert::AreEqual<uintptr_t>(view.m_BaseAddress, 0x000000006b800000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Size, 7);

		Assert::AreEqual(view.m_Sections.m_Array[0].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[0].m_Start, 0x00000000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[0].m_Size, 0x00010000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[0].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[1].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[1].m_Start, 0x00010000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[1].m_Size, 0x00065000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[1].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[2].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[2].m_Start, 0x00080000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[2].m_Size, 0x0002b000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[2].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[3].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[3].m_Start, 0x000b0000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[3].m_Size, 0x00001000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[3].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[4].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[4].m_Start, 0x000c0000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[4].m_Size, 0x00001000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[4].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[5].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[5].m_Start, 0x000d0000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[5].m_Size, 0x00001000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[5].m_Visuals.m_Size, 0);

		Assert::AreEqual(view.m_Sections.m_Array[6].m_Type, ExecutableViewSection::Type::Bytes);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[6].m_Start, 0x000e0000);
		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[6].m_Size, 0x00005000);

		Assert::AreEqual<uint32_t>(view.m_Sections.m_Array[6].m_Visuals.m_Size, 0);
	}
};