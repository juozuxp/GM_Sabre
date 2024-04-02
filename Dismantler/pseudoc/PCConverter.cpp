#include "PCConverter.hpp"

void PCConverter::Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const
{
	KaraBlob karaBlob;

	m_Kara.Convert(buffer, function, karaBlob);

	const KaraInstruction* conditional = nullptr;
	for (const KaraInstruction& ins : karaBlob.m_Instructions)
	{
		switch (ins.m_Type)
		{
		case KaraInstruction::Type::Subtract:
		{
			conditional = &ins;

			PCLine line;


		} break;
		case KaraInstruction::Type::Addition:
		{
			conditional = &ins;

		} break;
		case KaraInstruction::Type::Compare:
		{
			conditional = &ins;

		} break;
		case KaraInstruction::Type::AndCompare:
		{
			conditional = &ins;

		} break;
		case KaraInstruction::Type::Xor:
		{
			conditional = &ins;

		} break;
		case KaraInstruction::Type::Or:
		{
			conditional = &ins;

		} break;
		case KaraInstruction::Type::And:
		{
			conditional = &ins;

		} break;
		}
	}
}