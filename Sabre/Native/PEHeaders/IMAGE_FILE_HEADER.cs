using Microsoft.VisualStudio.OLE.Interop;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.PEHeaders
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct IMAGE_FILE_HEADER
	{
		public readonly ushort Machine;
		public readonly ushort NumberOfSections;
		public readonly uint TimeDateStamp;
		public readonly uint PointerToSymbolTable;
		public readonly uint NumberOfSymbols;
		public readonly ushort SizeOfOptionalHeader;
		public readonly ushort Characteristics;
	}
}
