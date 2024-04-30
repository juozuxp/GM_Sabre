using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Dismantler.Visuals
{
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct Value
	{
		public Scale m_Size;

		public byte m_Segment; // reserved for OperandMemoryValue

		public long m_Value;
	}
}
