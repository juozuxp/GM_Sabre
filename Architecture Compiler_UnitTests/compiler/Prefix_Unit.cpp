#include "pch.h"
#include "utility/Setup.hpp"
#include <compiler/Prefix.hpp>
#include <compiler/BytePackage.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Prefix_Unit)
{
public:
	TEST_METHOD(Construct)
	{
		Prefix prefix = Prefix(Prefix::Type_Lock | Prefix::Type_Wait);

		Assert::AreEqual(prefix.GetPackageType(), ByteEntry::PackageType::Prefix);
		Assert::AreEqual(prefix.GetRedirectPrefix(), Redirection::Prefix::Wait);

		prefix = Prefix(Prefix::Type_Lock | Prefix::Type_Repe);

		Assert::AreEqual(prefix.GetPackageType(), ByteEntry::PackageType::Prefix);
		Assert::AreEqual(prefix.GetRedirectPrefix(), Redirection::Prefix::Repe);

		prefix = Prefix(Prefix::Type_Lock | Prefix::Type_Repne);

		Assert::AreEqual(prefix.GetPackageType(), ByteEntry::PackageType::Prefix);
		Assert::AreEqual(prefix.GetRedirectPrefix(), Redirection::Prefix::Repne);

		prefix = Prefix(Prefix::Type_Lock | Prefix::Type_x66);

		Assert::AreEqual(prefix.GetPackageType(), ByteEntry::PackageType::Prefix);
		Assert::AreEqual(prefix.GetRedirectPrefix(), Redirection::Prefix::x66);

		prefix = Prefix(Prefix::Type_Lock | Prefix::Type_RexW);

		Assert::AreEqual(prefix.GetPackageType(), ByteEntry::PackageType::Prefix);
		Assert::AreEqual(prefix.GetRedirectPrefix(), Redirection::Prefix::RexW);
	}

	TEST_METHOD(SetStandAlone)
	{
		Prefix prefix = Prefix(Prefix::Type_Wait);

		Assert::IsFalse(prefix.IsStandAlone());

		prefix.SetStandAlone(0x10);

		Assert::IsTrue(prefix.IsStandAlone());
	}
};