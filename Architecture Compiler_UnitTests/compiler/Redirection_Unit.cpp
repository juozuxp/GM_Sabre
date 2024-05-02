#include "pch.h"
#include "utility/Setup.hpp"
#include <compiler/Prefix.hpp>
#include <compiler/BytePackage.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Redirection_Unit)
{
public:
	TEST_METHOD(Insert_0)
	{
		Instruction instruction = Instruction(Descriptor(Token("F7 /7 -> IDIV r/m32|16|64")), 123);

		std::vector<Redirection::Entry> entries;

		entries.push_back(Redirection::Entry(Redirection::Type::Mem, 4));
		entries.push_back(Redirection::Entry(Redirection::Type::Reg, 1));

		std::shared_ptr<Redirection> entry = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(nullptr, std::make_shared<Instruction>(instruction), entries));

		Assert::AreEqual(entry->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(entry->m_Type, Redirection::Type::Reg);

		std::shared_ptr<Redirection> mem = std::reinterpret_pointer_cast<Redirection>(entry->m_Redirects[1]);

		Assert::AreEqual(mem->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mem->m_Type, Redirection::Type::Mem);

		std::shared_ptr<Instruction> target = std::reinterpret_pointer_cast<Instruction>(mem->m_Redirects[4]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);

		entries.clear();

		entries.push_back(Redirection::Entry(Redirection::Prefix::x66));
		entries.push_back(Redirection::Entry(Redirection::X0F383A::x0F38));
		entries.push_back(Redirection::Entry(Redirection::Type::Mem, 2));
		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 1));
		entries.push_back(Redirection::Entry(Redirection::Type::Reg, 2));

		entry = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(entry, std::make_shared<Instruction>(instruction), entries));

		Assert::AreEqual(entry->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(entry->m_Type, Redirection::Type::X0F383A);

		std::shared_ptr<Redirection> prefix = std::reinterpret_pointer_cast<Redirection>(entry->m_Redirects[static_cast<uint8_t>(Redirection::X0F383A::x0F38)]);

		Assert::AreEqual(prefix->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(prefix->m_Type, Redirection::Type::Prefix);

		std::shared_ptr<Redirection> mod = std::reinterpret_pointer_cast<Redirection>(prefix->m_Redirects[static_cast<uint8_t>(Redirection::Prefix::x66)]);

		Assert::AreEqual(mod->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Type, Redirection::Type::Mod);

		std::shared_ptr<Redirection> reg = std::reinterpret_pointer_cast<Redirection>(mod->m_Redirects[1]);

		Assert::AreEqual(reg->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(reg->m_Type, Redirection::Type::Reg);

		mem = std::reinterpret_pointer_cast<Redirection>(reg->m_Redirects[2]);

		Assert::AreEqual(mem->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mem->m_Type, Redirection::Type::Mem);

		target = std::reinterpret_pointer_cast<Instruction>(mem->m_Redirects[2]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);
	}

	TEST_METHOD(Insert_1)
	{
		std::shared_ptr<Instruction> instruction = std::make_shared<Instruction>(Descriptor(Token("0F AF -> IMUL r32|16|64, r/m32|16|64")), 123);

		std::vector<Redirection::Entry> entries;

		std::shared_ptr<Instruction> target = std::reinterpret_pointer_cast<Instruction>(Redirection::Insert(nullptr, instruction, entries));

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);

		entries.push_back(Redirection::Entry(Redirection::X0F383A::x0F38));

		std::shared_ptr<Redirection> x0F383A = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(target, instruction, entries));

		Assert::AreEqual(x0F383A->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(x0F383A->m_Type, Redirection::Type::X0F383A);

		target = std::reinterpret_pointer_cast<Instruction>(x0F383A->m_Redirects[static_cast<uint8_t>(Redirection::X0F383A::x0F38)]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);

		entries.clear();

		entries.push_back(Redirection::Entry(Redirection::Prefix::Repe));

		x0F383A = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(x0F383A, instruction, entries));

		Assert::AreEqual(x0F383A->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(x0F383A->m_Type, Redirection::Type::X0F383A);

		std::shared_ptr<Redirection> prefix = std::reinterpret_pointer_cast<Redirection>(x0F383A->m_Redirects[static_cast<uint8_t>(Redirection::X0F383A::Default)]);

		Assert::AreEqual(prefix->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(prefix->m_Type, Redirection::Type::Prefix);

		target = std::reinterpret_pointer_cast<Instruction>(prefix->m_Redirects[static_cast<uint8_t>(Redirection::Prefix::Repe)]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);
	}

	TEST_METHOD(Insert_2)
	{
		std::shared_ptr<Instruction> fsub = std::make_shared<Instruction>(Descriptor(Token("DC /4 -> FSUB m64")), 123);
		std::shared_ptr<Instruction> div = std::make_shared<Instruction>(Descriptor(Token("F7 /7 -> IDIV r/m32|16|64")), 11);

		std::vector<Redirection::Entry> entries;

		entries.push_back(Redirection::Entry(Redirection::Prefix::RexW));
		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 2));

		std::shared_ptr<Redirection> prefix = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(nullptr, fsub, entries));

		Assert::AreEqual(prefix->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(prefix->m_Type, Redirection::Type::Prefix);

		std::shared_ptr<Redirection> mod = std::reinterpret_pointer_cast<Redirection>(prefix->m_Redirects[static_cast<uint8_t>(Redirection::Prefix::RexW)]);

		Assert::AreEqual(mod->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Type, Redirection::Type::Mod);

		std::shared_ptr<Instruction> target = std::reinterpret_pointer_cast<Instruction>(mod->m_Redirects[2]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);

		entries.clear();

		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 3));

		prefix = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(prefix, div, entries));

		Assert::AreEqual(prefix->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(prefix->m_Type, Redirection::Type::Prefix);

		mod = std::reinterpret_pointer_cast<Redirection>(prefix->m_Redirects[static_cast<uint8_t>(Redirection::Prefix::Default)]);

		Assert::AreEqual(mod->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Type, Redirection::Type::Mod);

		target = std::reinterpret_pointer_cast<Instruction>(mod->m_Redirects[3]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);
	}

	TEST_METHOD(Insert_3)
	{
		std::shared_ptr<Instruction> fsub = std::make_shared<Instruction>(Descriptor(Token("DC /4 -> FSUB m64")), 123);

		std::vector<Redirection::Entry> entries;

		entries.push_back(Redirection::Entry(Redirection::Type::Mod, 2));
		entries.push_back(Redirection::Entry(Redirection::Type::Reg, 2));

		std::shared_ptr<Redirection> mod = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(nullptr, fsub, entries));

		Assert::AreEqual(mod->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Type, Redirection::Type::Mod);

		std::shared_ptr<Redirection> reg = std::reinterpret_pointer_cast<Redirection>(mod->m_Redirects[2]);

		Assert::AreEqual(reg->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(reg->m_Type, Redirection::Type::Reg);

		std::shared_ptr<Instruction> target = std::reinterpret_pointer_cast<Instruction>(reg->m_Redirects[2]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);

		entries.clear();

		entries.push_back(Redirection::Entry(Redirection::Type::Reg, 3));

		mod = std::reinterpret_pointer_cast<Redirection>(Redirection::Insert(mod, fsub, entries));

		Assert::AreEqual(mod->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Type, Redirection::Type::Mod);

		Assert::AreEqual(mod->m_Redirects[0]->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Redirects[1]->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Redirects[2]->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(mod->m_Redirects[3]->GetPackageType(), ByteEntry::PackageType::Redirection);

		reg = std::reinterpret_pointer_cast<Redirection>(mod->m_Redirects[2]);

		Assert::AreEqual(reg->GetPackageType(), ByteEntry::PackageType::Redirection);
		Assert::AreEqual(reg->m_Type, Redirection::Type::Reg);

		target = std::reinterpret_pointer_cast<Instruction>(reg->m_Redirects[3]);

		Assert::AreEqual(target->GetPackageType(), ByteEntry::PackageType::Instruction);
	}
};