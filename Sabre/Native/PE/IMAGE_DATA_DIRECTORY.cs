using Microsoft.VisualStudio.OLE.Interop;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.PE
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct IMAGE_DATA_DIRECTORY
	{
		public readonly uint VirtualAddress;
		public readonly uint Size;
	}
}
