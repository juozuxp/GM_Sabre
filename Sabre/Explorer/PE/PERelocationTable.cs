using Sabre.Native.PE;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.PE
{
	internal struct PERelocationTable
	{
		public IMAGE_BASE_RELOCATION m_Descriptor;
		public PERelocationEntry[] m_Relocations;
	}
}
