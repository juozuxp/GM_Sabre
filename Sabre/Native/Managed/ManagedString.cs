using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.Managed
{
	[StructLayout(LayoutKind.Sequential)]
	public readonly struct ManagedString : IDisposable
	{
		private readonly ManagedObject m_Base;

		private readonly IntPtr m_Buffer;
		private readonly int m_Length;

		public void Dispose()
		{
			m_Base.Dispose();
		}

		public override string ToString()
		{
			if (m_Length == 0)
			{
				return string.Empty;
			}

			return Marshal.PtrToStringUni(m_Buffer, m_Length);
		}
	}
}
