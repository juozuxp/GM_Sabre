using System.Runtime.InteropServices;

namespace Sabre.Native.PE
{
	[StructLayout(LayoutKind.Sequential)]
	public struct IMAGE_OPTIONAL_HEADER32
	{
		const int IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16;

		public const int IMAGE_DIRECTORY_ENTRY_EXPORT = 0;
		public const int IMAGE_DIRECTORY_ENTRY_IMPORT = 1;
		public const int IMAGE_DIRECTORY_ENTRY_RESOURCE = 2;
		public const int IMAGE_DIRECTORY_ENTRY_EXCEPTION = 3;
		public const int IMAGE_DIRECTORY_ENTRY_SECURITY = 4;
		public const int IMAGE_DIRECTORY_ENTRY_BASERELOC = 5;
		public const int IMAGE_DIRECTORY_ENTRY_DEBUG = 6;
		public const int IMAGE_DIRECTORY_ENTRY_ARCHITECTURE = 7;
		public const int IMAGE_DIRECTORY_ENTRY_GLOBALPTR = 8;
		public const int IMAGE_DIRECTORY_ENTRY_TLS = 9;
		public const int IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG = 10;
		public const int IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT = 11;
		public const int IMAGE_DIRECTORY_ENTRY_IAT = 12;
		public const int IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT = 13;
		public const int IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR = 14;

		public readonly ushort Magic;
		public readonly byte MajorLinkerVersion;
		public readonly byte MinorLinkerVersion;
		public readonly uint SizeOfCode;
		public readonly uint SizeOfInitializedData;
		public readonly uint SizeOfUninitializedData;
		public readonly uint AddressOfEntryPoint;
		public readonly uint BaseOfCode;
		public readonly uint BaseOfData;
		public readonly uint ImageBase;
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
		public readonly uint SizeOfStackReserve;
		public readonly uint SizeOfStackCommit;
		public readonly uint SizeOfHeapReserve;
		public readonly uint SizeOfHeapCommit;
		public readonly uint LoaderFlags;
		public readonly uint NumberOfRvaAndSizes;

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = IMAGE_NUMBEROF_DIRECTORY_ENTRIES)]
		public readonly IMAGE_DATA_DIRECTORY[] DataDirectory;
	}
}
