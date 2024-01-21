using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Saber.Native
{
	[StructLayout(LayoutKind.Sequential)]
	internal readonly struct ManagedString : IDisposable
	{
		public readonly ManagedObject m_Base;

		private readonly IntPtr m_Buffer;
		private readonly int m_Length;

		public void Dispose()
		{
			m_Base.Dispose();
		}

		public override string ToString()
		{
			return Marshal.PtrToStringUni(m_Buffer, m_Length);
		}
	}
}
