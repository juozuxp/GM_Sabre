using Microsoft.VisualStudio.OLE.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Native.PEHeaders
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct IMAGE_IMPORT_DESCRIPTOR
	{
		public readonly uint OriginalFirstThunk;				// RVA to original unbound IAT (PIMAGE_THUNK_DATA)
		public readonly uint TimeDateStamp;						// 0 if not bound,
																// -1 if bound, and real date\time stamp
																//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
																// O.W. date/time stamp of DLL bound to (Old BIND)
		public readonly uint ForwarderChain;					// -1 if no forwarders
		public readonly uint Name;
		public readonly uint FirstThunk;
	}
}
