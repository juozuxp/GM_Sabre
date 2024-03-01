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
	internal struct IMAGE_DOS_HEADER
	{
		public readonly ushort e_magic;			// Magic number
		public readonly ushort e_cblp;			// Bytes on last page of file
		public readonly ushort e_cp;			// Pages in file
		public readonly ushort e_crlc;			// Relocations
		public readonly ushort e_cparhdr;		// Size of header in paragraphs
		public readonly ushort e_minalloc;		// Minimum extra paragraphs needed
		public readonly ushort e_maxalloc;		// Maximum extra paragraphs needed
		public readonly ushort e_ss;			// Initial (relative) SS value
		public readonly ushort e_sp;			// Initial SP value
		public readonly ushort e_csum;			// Checksum
		public readonly ushort e_ip;			// Initial IP value
		public readonly ushort e_cs;			// Initial (relative) CS value
		public readonly ushort e_lfarlc;		// File address of relocation table
		public readonly ushort e_ovno;          // Overlay number

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
		public readonly ushort[] e_res;			// Reserved words

		public readonly ushort e_oemid;			// OEM identifier (for e_oeminfo)
		public readonly ushort e_oeminfo;       // OEM information; e_oemid specific

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
		public readonly ushort[] e_res2;		// Reserved words
		
		public readonly int e_lfanew;			// File address of new exe header
	}
}
