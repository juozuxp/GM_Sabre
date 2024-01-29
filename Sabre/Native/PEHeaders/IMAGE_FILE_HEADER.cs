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
		public IMAGE_FILE_HEADER(MemoryStream stream)
		{
			BinaryReader reader = new BinaryReader(stream);

			Machine = reader.ReadUInt16();
			NumberOfSections = reader.ReadUInt16();

			TimeDateStamp = reader.ReadUInt32();
			PointerToSymbolTable = reader.ReadUInt32();
			NumberOfSymbols = reader.ReadUInt32();

			SizeOfOptionalHeader = reader.ReadUInt16();
			Characteristics = reader.ReadUInt16();
		}

		public ushort Machine;
		public ushort NumberOfSections;
		public uint TimeDateStamp;
		public uint PointerToSymbolTable;
		public uint NumberOfSymbols;
		public ushort SizeOfOptionalHeader;
		public ushort Characteristics;
	}
}
