using Microsoft.VisualStudio.Text.Differencing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Saber.Dismantler.Visuals
{
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	internal struct Instruction
	{
		public ushort m_Type;
		public byte m_Size;
	}
}
