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
	internal struct IMAGE_DELAYLOAD_DESCRIPTOR
	{
		public readonly uint AllAttributes;
		public readonly uint DllNameRVA;						// RVA to the name of the target library (NULL-terminate ASCII string)
		public readonly uint ModuleHandleRVA;					// RVA to the HMODULE caching location (PHMODULE)
		public readonly uint ImportAddressTableRVA;				// RVA to the start of the IAT (PIMAGE_THUNK_DATA)
		public readonly uint ImportNameTableRVA;				// RVA to the start of the name table (PIMAGE_THUNK_DATA::AddressOfData)
		public readonly uint BoundImportAddressTableRVA;		// RVA to an optional bound IAT
		public readonly uint UnloadInformationTableRVA;         // RVA to an optional unload info table
		public readonly uint TimeDateStamp;						// 0 if not bound,
																// Otherwise, date/time of the target DLL
	}
}
