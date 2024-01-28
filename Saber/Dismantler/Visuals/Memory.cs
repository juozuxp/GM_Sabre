﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Saber.Dismantler.Visuals
{
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	internal struct Memory
	{
		public Scale m_Size;

		public byte m_Base;
		public byte m_Index;
		public byte m_Segment;
		public byte m_Multiplier;

		public int m_Offset;
	}
}
