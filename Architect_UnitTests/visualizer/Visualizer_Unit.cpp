#include "pch.h"
#include "utility/Setup.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Visualizer_Unit)
{
private:
	enum VisRegisterBase
	{
		VisReg_General_64 = 0,
		VisReg_General_32 = 16,
		VisReg_General_16 = 32,
		VisReg_General_8l = 48,
		VisReg_General_8h = 64,
		VisReg_SSE_128 = 68,
		VisReg_MMX_64 = 84,
		VisReg_MPX_64 = 100,
		VisReg_FPU_80 = 104,
		VisReg_CR_64 = 112,
		VisReg_DR_64 = 128,
		VisReg_Sreg_64 = 144,
	};

public:
	TEST_METHOD(Visualize_add)
	{
		std::vector<ILInstruction> instructions;

		ILInstruction instruction;

		instruction.m_Size = 0x50;
		instruction.m_Type = InsType_add;

		instruction.m_Operands[0].m_Type = ILOperandType_Memory;
		instruction.m_Operands[0].m_Scale = ILOperandScale_32;

		instruction.m_Operands[0].m_Memory.m_Segment = REG_GS;
		instruction.m_Operands[0].m_Memory.m_Scale = ILMemoryScaler_4;

		instruction.m_Operands[0].m_Memory.m_Base = REG_RAX;
		instruction.m_Operands[0].m_Memory.m_Index = REG_R8;

		instruction.m_Operands[0].m_Memory.m_Offset = 0x10;

		instruction.m_Operands[1].m_Type = ILOperandType_Value;
		instruction.m_Operands[1].m_Scale = ILOperandScale_8;

		instruction.m_Operands[1].m_Value = 0x32;

		instruction.m_Operands[2].m_Type = ILOperandType_Register;
		instruction.m_Operands[2].m_Scale = ILOperandScale_128;

		instruction.m_Operands[2].m_Register.m_Base = REG_XMM1;
		instruction.m_Operands[2].m_Register.m_BaseHigh = false;

		instruction.m_Operands[2].m_Register.m_Type = Register::xmm;

		instruction.m_Operands[3].m_Type = ILOperandType_ValueRelative;
		instruction.m_Operands[3].m_Scale = ILOperandScale_8;

		instruction.m_Operands[3].m_Relative.m_Value = 0x10;
		instruction.m_Operands[3].m_Relative.m_Segment = IL_INVALID_REGISTER;

		instructions.push_back(instruction);

		Visualizer visualizer;

		std::vector<Visualizer::Visual> visuals = visualizer.ToVisuals(instructions, reinterpret_cast<const void*>(1ull << 63));

		Assert::AreEqual<size_t>(visuals.size(), 5);

		const Visualizer::Visual& first = visuals[0];

		Assert::AreEqual(first.m_Type, Visualizer::VisType::Instruction);

		Assert::AreEqual(first.m_Instruction.m_Type, instruction.m_Type);
		Assert::AreEqual<uint8_t>(first.m_Instruction.m_Size, instruction.m_Size);

		const Visualizer::Visual& second = visuals[1];

		Assert::AreEqual(second.m_Type, Visualizer::VisType::OperandMemory);

		Assert::AreEqual(second.m_Memory.m_Size, ILOperandScale_32);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Base, VisReg_General_64 + REG_RAX);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Index, VisReg_General_64 + REG_R8);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Segment, VisReg_Sreg_64 + REG_GS);
		Assert::AreEqual<uint8_t>(second.m_Memory.m_Multiplier, 4);
		Assert::AreEqual<int32_t>(second.m_Memory.m_Offset, 0x10);

		const Visualizer::Visual& thrid = visuals[2];

		Assert::AreEqual(thrid.m_Type, Visualizer::VisType::OperandValue);

		Assert::AreEqual(thrid.m_Value.m_Size, ILOperandScale_8);
		Assert::AreEqual<uint8_t>(thrid.m_Value.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(thrid.m_Value.m_Value, 0x32);

		const Visualizer::Visual& fourth = visuals[3];

		Assert::AreEqual(fourth.m_Type, Visualizer::VisType::OperandRegister);

		Assert::AreEqual<uint8_t>(fourth.m_Register, VisReg_SSE_128 + REG_XMM1);

		const Visualizer::Visual& fifth = visuals[4];

		Assert::AreEqual(fifth.m_Type, Visualizer::VisType::OperandAddressValue);

		Assert::AreEqual(fifth.m_Value.m_Size, ILOperandScale_8);
		Assert::AreEqual<uint8_t>(fifth.m_Value.m_Segment, VIS_INVALID_REGISTER);
		Assert::AreEqual<int64_t>(fifth.m_Value.m_Value, (1ull << 63) + 0x10 + instruction.m_Size);
	}
};