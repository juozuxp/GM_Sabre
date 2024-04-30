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
	public struct IMAGE_NT_HEADERS32
	{
		public readonly uint Signature;
		public readonly IMAGE_FILE_HEADER FileHeader;
		public readonly IMAGE_OPTIONAL_HEADER32 OptionalHeader;
	}
}
