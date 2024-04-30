using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	[StructLayout(LayoutKind.Sequential)]
	public struct PERelocationEntry
	{
		public byte m_Type;
		public ushort m_Offset;
		public ulong m_Value;
	}
}
