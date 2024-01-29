using Microsoft.VisualStudio.Threading;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.Managed
{
	[StructLayout(LayoutKind.Sequential)]
	internal readonly struct ManagedArray : IDisposable
	{
		private readonly ManagedObject m_Base;

		private readonly IntPtr m_Array;
		private readonly int m_Size;
		private readonly int m_Capacity;

		public void Dispose()
		{
			m_Base.Dispose();
		}

		public T[] ToArray<T>()
		{
			T[] array = new T[m_Size];

			IntPtr address = m_Array;
			for (int i = 0; i < m_Size; i++, address += IntPtr.Size)
			{
				array[i] = Marshal.PtrToStructure<T>(Marshal.ReadIntPtr(address));
			}

			return array;
		}
	}
}
