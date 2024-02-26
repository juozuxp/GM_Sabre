using Sabre.Dismantler.Visuals;
using Sabre.Native.Managed;
using Sabre.Native.PEHeaders;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.PortableExecutable;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer
{
	internal class ExecutableExplorer
	{
		internal struct NativePEImportEntry : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly ushort m_Ordinal;
			public readonly uint m_FirstThunk;
			public readonly uint m_OriginalFirstThunk;

			public readonly ManagedString m_Name;

			public PEImportEntry ToData()
			{
				PEImportEntry data = new PEImportEntry();

				data.m_Ordinal = m_Ordinal;
				data.m_FirstThunk = m_FirstThunk;
				data.m_OriginalFirstThunk = m_OriginalFirstThunk;

				data.m_Name = m_Name.ToString();

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativePEImportTable : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IMAGE_IMPORT_DESCRIPTOR m_Descriptor;

			public readonly ManagedString m_Name;
			public readonly ManagedGenericArray m_Entries; // NativePEImportEntry

			public PEImportTable ToData()
			{
				PEImportTable data = new PEImportTable();

				data.m_Descriptor = m_Descriptor;
				data.m_Name = m_Name.ToString();

				NativePEImportEntry[] entries = m_Entries.ToArray<NativePEImportEntry>();

				data.m_Entries = new PEImportEntry[entries.Length];
				for (int i = 0; i < entries.Length; i++)
				{
					data.m_Entries[i] = entries[i].ToData();
				}

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativeHeaders : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IntPtr m_BaseAddress;

			public readonly IMAGE_DOS_HEADER m_Dos;
			public readonly IMAGE_NT_HEADERS32 m_NT32;
			public readonly IMAGE_NT_HEADERS64 m_NT64;

			public readonly ManagedGenericArray m_Imports; // NativePEImportTable
			public readonly ManagedGenericArray m_Sections; // IMAGE_SECTION_HEADER

			public PEHeaders ToData()
			{
				PEHeaders data = new PEHeaders();

				data.m_Dos = m_Dos;
				data.m_NT32 = m_NT32;
				data.m_NT64 = m_NT64;

				data.m_Base = m_BaseAddress;

				NativePEImportTable[] imports = m_Imports.ToArray<NativePEImportTable>();

				data.m_Imports = new PEImportTable[imports.Length];
				for (int i = 0; i < imports.Length; i++)
				{
					data.m_Imports[i] = imports[i].ToData();
				}

				data.m_Sections = m_Sections.ToArray<IMAGE_SECTION_HEADER>();

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativeExecutableViewSection : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly ExecutableViewSection.Type m_Type;

			public readonly uint m_Start;
			public readonly uint m_Size;

			public readonly ManagedGenericArray m_Visuals;

			public ExecutableViewSection ToData()
			{
				ExecutableViewSection data = new ExecutableViewSection();

				data.m_Type = m_Type;

				data.m_Size = m_Size;
				data.m_Start = m_Start;

				data.m_Visuals = m_Visuals.ToArray<NativeVisual>();

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativeExecutableView : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IntPtr m_BaseAddress; // image base address
			public readonly IntPtr m_DataAddress; // actual mapped address

			public readonly ManagedGenericArray m_Sections;

			public ExecutableView ToData()
			{
				ExecutableView data = new ExecutableView();

				data.m_DataAddress = m_DataAddress;
				data.m_BaseAddress = m_BaseAddress;

				data.m_Sections = new ExecutableViewSection[m_Sections.m_Size];

				NativeExecutableViewSection[] sections = m_Sections.ToArray<NativeExecutableViewSection>();
				for (int i = 0; i < sections.Length; i++)
				{
					data.m_Sections[i] = sections[i].ToData();
				}

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[DllImport("Dismantler.dll", CharSet = CharSet.Unicode)]
		private static extern IntPtr ExecutableExplorer_Init(string path);

		[DllImport("Dismantler.dll")]
		private static extern IntPtr ExecutableExplorer_GetHeaders(IntPtr instance);

		[DllImport("Dismantler.dll")]
		private static extern IntPtr ExecutableExplorer_GetExecutableView(IntPtr instance);

		private readonly ManagedObject m_Instance;

		public ExecutableExplorer(string path) 
		{
			m_Instance = Marshal.PtrToStructure<ManagedObject>(ExecutableExplorer_Init(path));
		}

		~ExecutableExplorer() 
		{
			m_Instance.Dispose();
		}

		public PEHeaders? GetHeaders()
		{
			IntPtr pointer = ExecutableExplorer_GetHeaders(m_Instance);
			if (pointer == IntPtr.Zero)
			{
				return null;
			}

			using (NativeHeaders native = Marshal.PtrToStructure<NativeHeaders>(pointer))
			{
				return native.ToData();
			}
		}

		public ExecutableView? GetExecutableView()
		{
			IntPtr pointer = ExecutableExplorer_GetExecutableView(m_Instance);
			if (pointer == IntPtr.Zero)
			{
				return null;
			}

			using (NativeExecutableView native = Marshal.PtrToStructure<NativeExecutableView>(pointer))
			{
				return native.ToData();
			}
		}
	}
}
