using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Containers
{
	internal class RollingStack<T>
	{
		private int m_Count;
		private int m_Cursor;
		private readonly T[] m_Stack;

		public RollingStack(int size)
		{
			m_Cursor = 0;
			m_Stack = new T[size];
		}

		public void Reset()
		{
			m_Count = 0;
			m_Cursor = 0;
		}

		public void Push(T item)
		{
			m_Stack[m_Cursor++] = item;
			if (m_Cursor >= m_Stack.Length)
			{
				m_Cursor = 0;
			}
			
			if (m_Count < m_Stack.Length)
			{
				m_Count++;
			}
		}

		public T Pop()
		{
			if (m_Count <= 0)
			{
				throw new Exception("Nothing to pop");
			}

			m_Count--;
			return m_Stack[m_Cursor--];
		}

		public bool TryPop(out T value)
		{
			if (m_Count <= 0)
			{
				value = default;
				return false;
			}

			if (m_Cursor <= 0)
			{
				m_Cursor = m_Stack.Length;
			}

			m_Count--;
			value = m_Stack[--m_Cursor];
			return true;
		}
	}
}
