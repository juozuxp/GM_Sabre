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
	internal struct IMAGE_DOS_HEADER
	{
		public IMAGE_DOS_HEADER(MemoryStream stream)
		{
			BinaryReader reader = new BinaryReader(stream);

			e_magic = reader.ReadUInt16();
			e_cblp = reader.ReadUInt16();
			e_cp = reader.ReadUInt16();
			e_crlc = reader.ReadUInt16();
			e_cparhdr = reader.ReadUInt16();
			e_minalloc = reader.ReadUInt16();
			e_maxalloc = reader.ReadUInt16();
			e_ss = reader.ReadUInt16();
			e_sp = reader.ReadUInt16();
			e_csum = reader.ReadUInt16();
			e_ip = reader.ReadUInt16();
			e_cs = reader.ReadUInt16();
			e_lfarlc = reader.ReadUInt16();
			e_ovno = reader.ReadUInt16();

			e_res = new ushort[4];
			for (int i = 0; i < e_res.Length; i++)
			{
				e_res[i] = reader.ReadUInt16();
			}

			e_oemid = reader.ReadUInt16();
			e_oeminfo = reader.ReadUInt16();

			e_res2 = new ushort[10];
			for (int i = 0; i < e_res2.Length; i++)
			{
				e_res2[i] = reader.ReadUInt16();
			}

			e_lfanew = reader.ReadUInt16();
		}

		public ushort e_magic;			// Magic number
		public ushort e_cblp;			// Bytes on last page of file
		public ushort e_cp;				// Pages in file
		public ushort e_crlc;			// Relocations
		public ushort e_cparhdr;		// Size of header in paragraphs
		public ushort e_minalloc;		// Minimum extra paragraphs needed
		public ushort e_maxalloc;		// Maximum extra paragraphs needed
		public ushort e_ss;				// Initial (relative) SS value
		public ushort e_sp;				// Initial SP value
		public ushort e_csum;			// Checksum
		public ushort e_ip;				// Initial IP value
		public ushort e_cs;				// Initial (relative) CS value
		public ushort e_lfarlc;			// File address of relocation table
		public ushort e_ovno;           // Overlay number

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
		private ushort[] e_res;			// Reserved words

		public ushort e_oemid;			// OEM identifier (for e_oeminfo)
		public ushort e_oeminfo;        // OEM information; e_oemid specific

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
		private ushort[] e_res2;		// Reserved words
		
		public int e_lfanew;			// File address of new exe header
	}
}
