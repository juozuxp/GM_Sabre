using System.Runtime.InteropServices;

namespace Sabre.Native.PE
{
	[StructLayout(LayoutKind.Sequential)]
	internal struct IMAGE_OPTIONAL_HEADER64
	{
		const int IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16;

		const int IMAGE_DIRECTORY_ENTRY_EXPORT = 0;
		const int IMAGE_DIRECTORY_ENTRY_IMPORT = 1;
		const int IMAGE_DIRECTORY_ENTRY_RESOURCE = 2;
		const int IMAGE_DIRECTORY_ENTRY_EXCEPTION = 3;
		const int IMAGE_DIRECTORY_ENTRY_SECURITY = 4;
		const int IMAGE_DIRECTORY_ENTRY_BASERELOC = 5;
		const int IMAGE_DIRECTORY_ENTRY_DEBUG = 6;
		const int IMAGE_DIRECTORY_ENTRY_ARCHITECTURE = 7;
		const int IMAGE_DIRECTORY_ENTRY_GLOBALPTR = 8;
		const int IMAGE_DIRECTORY_ENTRY_TLS = 9;
		const int IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG = 10;
		const int IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT = 11;
		const int IMAGE_DIRECTORY_ENTRY_IAT = 12;
		const int IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT = 13;
		const int IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR = 14;

		public readonly ushort Magic;
		public readonly byte MajorLinkerVersion;
		public readonly byte MinorLinkerVersion;
		public readonly uint SizeOfCode;
		public readonly uint SizeOfInitializedData;
		public readonly uint SizeOfUninitializedData;
		public readonly uint AddressOfEntryPoint;
		public readonly uint BaseOfCode;
		public readonly ulong ImageBase;
		public readonly uint SectionAlignment;
		public readonly uint FileAlignment;
		public readonly ushort MajorOperatingSystemVersion;
		public readonly ushort MinorOperatingSystemVersion;
		public readonly ushort MajorImageVersion;
		public readonly ushort MinorImageVersion;
		public readonly ushort MajorSubsystemVersion;
		public readonly ushort MinorSubsystemVersion;
		public readonly uint Win32VersionValue;
		public readonly uint SizeOfImage;
		public readonly uint SizeOfHeaders;
		public readonly uint CheckSum;
		public readonly ushort Subsystem;
		public readonly ushort DllCharacteristics;
		public readonly ulong SizeOfStackReserve;
		public readonly ulong SizeOfStackCommit;
		public readonly ulong SizeOfHeapReserve;
		public readonly ulong SizeOfHeapCommit;
		public readonly uint LoaderFlags;
		public readonly uint NumberOfRvaAndSizes;

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = IMAGE_NUMBEROF_DIRECTORY_ENTRIES)]
		public readonly IMAGE_DATA_DIRECTORY[] DataDirectory;
	}
}
