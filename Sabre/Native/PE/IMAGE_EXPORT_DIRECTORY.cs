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
	internal struct IMAGE_EXPORT_DIRECTORY
	{
		public readonly uint Characteristics;
		public readonly uint TimeDateStamp;
		public readonly ushort MajorVersion;
		public readonly ushort MinorVersion;
		public readonly uint Name;
		public readonly uint Base;
		public readonly uint NumberOfFunctions;
		public readonly uint NumberOfNames;
		public readonly uint AddressOfFunctions;		// RVA from base of image
		public readonly uint AddressOfNames;			// RVA from base of image
		public readonly uint AddressOfNameOrdinals;		// RVA from base of image
	}
}
