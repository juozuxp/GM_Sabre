#include "pch.h"
#include <CppUnitTest.h>
#include <managed/ManagedArray.hpp>

#include "utility/Object.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ManagedArray_Unit)
{
public:
	TEST_METHOD(Capacity_Construct)
	{
		ManagedArray<int32_t> managed = ManagedArray<int32_t>(100);

		Assert::IsTrue(managed.m_Capacity >= 100);
		Assert::AreEqual<uint32_t>(managed.m_Size, 0);
		Assert::AreNotEqual<int32_t*>(managed.m_Array, nullptr);
	}

	TEST_METHOD(vector_Construct)
	{
		std::vector<int32_t> items;

		items.push_back(100);
		items.push_back(200);
		items.push_back(500);

		ManagedArray<int32_t> managed = ManagedArray<int32_t>(items);

		Assert::IsTrue(managed.m_Capacity >= items.size());
		Assert::AreEqual<int32_t>(managed.m_Size, items.size());
		Assert::AreNotEqual<int32_t*>(managed.m_Array, nullptr);

		Assert::AreEqual<int32_t>(managed.m_Array[0], items[0]);
		Assert::AreEqual<int32_t>(managed.m_Array[1], items[1]);
		Assert::AreEqual<int32_t>(managed.m_Array[2], items[2]);
	}

	TEST_METHOD(Move_Equal)
	{
		std::vector<int32_t> items;

		items.push_back(100);
		items.push_back(200);
		items.push_back(500);

		ManagedArray<int32_t> original = ManagedArray<int32_t>(items);

		uint32_t size = original.m_Size;
		int32_t* array = original.m_Array;
		uint32_t capacity = original.m_Capacity;

		ManagedArray<int32_t> equal;

		equal = std::move(original);

		Assert::AreEqual(equal.m_Size, size);
		Assert::AreEqual(equal.m_Array, array);
		Assert::AreEqual(equal.m_Capacity, capacity);

		Assert::AreEqual<uint32_t>(original.m_Size, 0);
		Assert::AreEqual<uint32_t>(original.m_Capacity, 0);
		Assert::AreEqual<int32_t*>(original.m_Array, nullptr);
	}

	TEST_METHOD(Move_Construct)
	{
		std::vector<int32_t> items;

		items.push_back(100);
		items.push_back(200);
		items.push_back(500);

		ManagedArray<int32_t> original = ManagedArray<int32_t>(items);

		uint32_t size = original.m_Size;
		int32_t* array = original.m_Array;
		uint32_t capacity = original.m_Capacity;

		ManagedArray<int32_t> constructor = ManagedArray<int32_t>(std::move(original));

		Assert::AreEqual(constructor.m_Size, size);
		Assert::AreEqual(constructor.m_Array, array);
		Assert::AreEqual(constructor.m_Capacity, capacity);

		Assert::AreEqual<uint32_t>(original.m_Size, 0);
		Assert::AreEqual<uint32_t>(original.m_Capacity, 0);
		Assert::AreEqual<int32_t*>(original.m_Array, nullptr);
	}

	TEST_METHOD(Add_ints)
	{
		ManagedArray<int32_t> ints = ManagedArray<int32_t>(2);

		ints.Add(100);
		ints.Add(200);

		Assert::IsTrue(ints.m_Size >= 2);
		Assert::AreEqual<uint32_t>(ints.m_Size, 2);

		Assert::AreEqual<int32_t>(ints.m_Array[0], 100);
		Assert::AreEqual<int32_t>(ints.m_Array[1], 200);

		ints.Add(300);

		Assert::IsTrue(ints.m_Capacity >= 3);
		Assert::AreEqual<uint32_t>(ints.m_Size, 3);

		Assert::AreEqual<int32_t>(ints.m_Array[0], 100);
		Assert::AreEqual<int32_t>(ints.m_Array[1], 200);
		Assert::AreEqual<int32_t>(ints.m_Array[2], 300);
	}

	TEST_METHOD(Add_objects)
	{
		ManagedArray<Object> objects;

		Object move;
		objects.Add(std::move(move));

		Object copy;
		objects.Add(copy);

		Assert::IsTrue(objects.m_Capacity >= 2);
		Assert::AreEqual<uint32_t>(objects.m_Size, 2);

		Assert::IsTrue(objects.m_Array[0].m_Move);
		Assert::IsTrue(objects.m_Array[1].m_Copy);
	}

	TEST_METHOD(Reserve)
	{
		ManagedArray<int32_t> array = ManagedArray<int32_t>(2);

		array.Add(100);
		array.Add(200);

		array.Reserve(0);

		Assert::IsTrue(array.m_Capacity >= 2);
		Assert::AreEqual<uint32_t>(array.m_Size, 2);

		Assert::AreEqual<int32_t>(array.m_Array[0], 100);
		Assert::AreEqual<int32_t>(array.m_Array[1], 200);

		array.Reserve(10);

		Assert::IsTrue(array.m_Capacity >= 10);
		Assert::AreEqual<uint32_t>(array.m_Size, 2);

		Assert::AreEqual<int32_t>(array.m_Array[0], 100);
		Assert::AreEqual<int32_t>(array.m_Array[1], 200);
	}
};