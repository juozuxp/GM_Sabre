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
	internal struct IMAGE_NT_HEADERS64
	{
		public IMAGE_NT_HEADERS64(MemoryStream stream)
		{
			BinaryReader reader = new BinaryReader(stream);

			Signature = reader.ReadUInt32();
			FileHeader = new IMAGE_FILE_HEADER(stream);
			OptionalHeader = new IMAGE_OPTIONAL_HEADER64(stream);
		}

		public uint Signature;
		public IMAGE_FILE_HEADER FileHeader;
		public IMAGE_OPTIONAL_HEADER64 OptionalHeader;
	}
}
