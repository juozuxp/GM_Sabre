using Microsoft.VisualStudio.OLE.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.PEHeaders
{
	[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
	internal struct IMAGE_SECTION_HEADER
	{
		const int IMAGE_SIZEOF_SHORT_NAME = 8;

		[MarshalAs(UnmanagedType.ByValTStr, SizeConst = IMAGE_SIZEOF_SHORT_NAME)]
		public readonly string Name;

		public readonly uint VirtualSize;
		public readonly uint VirtualAddress;
		public readonly uint SizeOfRawData;
		public readonly uint PointerToRawData;
		public readonly uint PointerToRelocations;
		public readonly uint PointerToLinenumbers;
		public readonly ushort NumberOfRelocations;
		public readonly ushort NumberOfLinenumbers;
		public readonly uint Characteristics;
	}
}
