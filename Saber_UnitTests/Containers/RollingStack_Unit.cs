using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Containers;
using System;
using System.Reflection;

namespace Saber_Unit.Containers
{
	[TestClass]
	public class RollingStack_Unit
	{
		private readonly FieldInfo m_Count;
		private readonly FieldInfo m_Stack;
		private readonly FieldInfo m_Cursor;

		public RollingStack_Unit()
		{
			Type type = typeof(RollingStack<int>);

			m_Count = type.GetField("m_Count", BindingFlags.NonPublic | BindingFlags.Instance);
			m_Stack = type.GetField("m_Stack", BindingFlags.NonPublic | BindingFlags.Instance);
			m_Cursor = type.GetField("m_Cursor", BindingFlags.NonPublic | BindingFlags.Instance);
		}

		[TestMethod]
		public void Construct()
		{
			RollingStack<int> container = new RollingStack<int>(10);

			int count = (int)m_Count.GetValue(container);
			int cursor = (int)m_Cursor.GetValue(container);
			int[] stack = (int[])m_Stack.GetValue(container);

			Assert.IsNotNull(stack);
			Assert.AreEqual(count, 0);
			Assert.AreEqual(cursor, 0);
		}

		[TestMethod]
		public void Push()
		{
			RollingStack<int> stack = new RollingStack<int>(5);

			stack.Push(40);
			stack.Push(20);
			stack.Push(30);

			int count = (int)m_Count.GetValue(stack);
			int cursor = (int)m_Cursor.GetValue(stack);

			Assert.AreEqual(count, 3);
			Assert.AreEqual(cursor, 3);

			stack.Push(20);
			stack.Push(30);
			stack.Push(20);
			stack.Push(30);

			count = (int)m_Count.GetValue(stack);
			cursor = (int)m_Cursor.GetValue(stack);

			Assert.AreEqual(count, 5);
			Assert.AreEqual(cursor, 2);
		}

		[TestMethod]
		public void Push_roll()
		{
			RollingStack<int> stack = new RollingStack<int>(5);

			stack.Push(40);
			stack.Push(20);
			stack.Push(30);
			stack.Push(20);
			stack.Push(30);
			stack.Push(20);
			stack.Push(30);

			int count = (int)m_Count.GetValue(stack);
			int cursor = (int)m_Cursor.GetValue(stack);

			Assert.AreEqual(count, 5);
			Assert.AreEqual(cursor, 2);
		}

		[TestMethod]
		public void Pop()
		{
			RollingStack<int> stack = new RollingStack<int>(5);

			stack.Push(50);
			stack.Push(40);
			stack.Push(30);
			stack.Push(20);

			Assert.AreEqual(stack.Pop(), 20);
			Assert.AreEqual(stack.Pop(), 30);
			Assert.AreEqual(stack.Pop(), 40);
			Assert.AreEqual(stack.Pop(), 50);

			bool except = false;
			try
			{
				stack.Pop();
			}
			catch 
			{
				except = true;
			}

			Assert.IsTrue(except);

			stack.Push(50);
			stack.Push(40);
			stack.Push(30);
			stack.Push(20);
			stack.Push(10);
			stack.Push(0);

			Assert.AreEqual(stack.Pop(), 0);
			Assert.AreEqual(stack.Pop(), 10);
			Assert.AreEqual(stack.Pop(), 20);
			Assert.AreEqual(stack.Pop(), 30);
			Assert.AreEqual(stack.Pop(), 40);

			except = false;
			try
			{
				stack.Pop();
			}
			catch
			{
				except = true;
			}

			Assert.IsTrue(except);
		}

		[TestMethod]
		public void Pop_roll()
		{
			RollingStack<int> stack = new RollingStack<int>(5);

			stack.Push(50);
			stack.Push(40);
			stack.Push(30);
			stack.Push(20);
			stack.Push(10);
			stack.Push(0);

			Assert.AreEqual(stack.Pop(), 0);
			Assert.AreEqual(stack.Pop(), 10);
			Assert.AreEqual(stack.Pop(), 20);
			Assert.AreEqual(stack.Pop(), 30);
			Assert.AreEqual(stack.Pop(), 40);

			bool except = false;
			try
			{
				stack.Pop();
			}
			catch
			{
				except = true;
			}

			Assert.IsTrue(except);
		}

		[TestMethod]
		public void TryPop()
		{
			RollingStack<int> stack = new RollingStack<int>(5);

			stack.Push(50);
			stack.Push(40);
			stack.Push(30);
			stack.Push(20);

			int value;

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 20);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 30);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 40);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 50);

			Assert.IsFalse(stack.TryPop(out value));

			stack.Push(50);
			stack.Push(40);
			stack.Push(30);
			stack.Push(20);
			stack.Push(10);
			stack.Push(0);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 0);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 10);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 20);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 30);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 40);

			Assert.IsFalse(stack.TryPop(out value));
		}

		[TestMethod]
		public void TryPop_roll()
		{
			RollingStack<int> stack = new RollingStack<int>(5);

			stack.Push(50);
			stack.Push(40);
			stack.Push(30);
			stack.Push(20);
			stack.Push(10);
			stack.Push(0);

			int value;

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 0);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 10);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 20);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 30);

			Assert.IsTrue(stack.TryPop(out value));
			Assert.AreEqual(value, 40);

			Assert.IsFalse(stack.TryPop(out value));
		}

		[TestMethod]
		public void Reset()
		{
			RollingStack<int> stack = new RollingStack<int>(10);

			stack.Push(10);
			stack.Push(20);
			stack.Push(00);
			stack.Push(40);
			stack.Push(80);

			stack.Reset();

			int count = (int)m_Count.GetValue(stack);
			int cursor = (int)m_Cursor.GetValue(stack);

			Assert.AreEqual(count, 0);
			Assert.AreEqual(cursor, 0);
		}
	}
}
