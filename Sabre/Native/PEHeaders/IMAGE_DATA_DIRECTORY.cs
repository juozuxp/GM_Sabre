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
	internal struct IMAGE_DATA_DIRECTORY
	{
		public IMAGE_DATA_DIRECTORY(MemoryStream stream)
		{
			BinaryReader reader = new BinaryReader(stream);

			VirtualAddress = reader.ReadUInt32();
			Size = reader.ReadUInt32();
		}

		public uint VirtualAddress;
		public uint Size;
	}
}
