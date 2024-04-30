using Microsoft.VisualStudio.OLE.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.PE
{
	[StructLayout(LayoutKind.Sequential)]
	public struct IMAGE_BASE_RELOCATION
	{
		public enum Type
		{
			IMAGE_REL_BASED_ABSOLUTE = 0,
			IMAGE_REL_BASED_HIGH = 1,
			IMAGE_REL_BASED_LOW = 2,
			IMAGE_REL_BASED_HIGHLOW = 3,
			IMAGE_REL_BASED_HIGHADJ = 4,
			IMAGE_REL_BASED_MIPS_JMPADDR = 5,
			IMAGE_REL_BASED_THUMB_MOV32 = 7,
			IMAGE_REL_BASED_RISCV_LOW12S = 8,
			IMAGE_REL_BASED_MIPS_JMPADDR16 = 9,
			IMAGE_REL_BASED_DIR64 = 10
		}

		public readonly uint VirtualAddress;
		public readonly uint SizeOfBlock;
	}
}
