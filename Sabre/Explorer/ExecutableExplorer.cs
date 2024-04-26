using Sabre.Dismantler.Visuals;
using Sabre.Explorer.PE;
using Sabre.Native.Managed;
using Sabre.Native.PE;
using System;
using System.Runtime.InteropServices;
using Sabre.Explorer.Objects;
using static Sabre.Explorer.ExecutableExplorer;

namespace Sabre.Explorer
{
	internal class ExecutableExplorer
	{
		[StructLayout(LayoutKind.Sequential)]
		internal struct NativePEImportEntry : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly ushort m_Ordinal;
			public readonly uint m_FunctionAddressBase;

			public readonly ManagedString m_Name;

			public PEImportEntry ToData()
			{
				PEImportEntry data = new PEImportEntry();

				data.m_Ordinal = m_Ordinal;
				data.m_FunctionAddressBase = m_FunctionAddressBase;

				data.m_Name = m_Name.ToString();

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		struct NativePEExportEntry : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly ushort m_Ordinal;
			public readonly uint m_Function;

			public readonly ManagedString m_Name;
			public readonly ManagedString m_Forwarder;

			public PEExportEntry ToData()
			{
				PEExportEntry data = new PEExportEntry();

				data.m_Ordinal = m_Ordinal;
				data.m_Function = m_Function;

				data.m_Name = m_Name.ToString();
				data.m_Forwarder = m_Forwarder.ToString();

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativePEExportTable : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IMAGE_EXPORT_DIRECTORY m_Descriptor;

			public readonly ManagedString m_Name;
			public readonly ManagedArray m_Entries; // NativePEExportEntry

			public PEExportTable ToData()
			{
				PEExportTable data = new PEExportTable();

				data.m_Descriptor = m_Descriptor;
				data.m_Name = m_Name.ToString();

				NativePEExportEntry[] entries = m_Entries.ToArray<NativePEExportEntry>();

				data.m_Entries = new PEExportEntry[entries.Length];
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
		internal struct NativePEImportTable : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IMAGE_IMPORT_DESCRIPTOR m_Descriptor;

			public readonly ManagedString m_Name;
			public readonly ManagedArray m_Entries; // NativePEImportEntry

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
		struct NativePEDelayImportTable : IDisposable
		{
			private readonly ManagedObject m_Base;

			private readonly IMAGE_DELAYLOAD_DESCRIPTOR m_Descriptor;

			private readonly ManagedString m_Name;
			private readonly ManagedArray m_Entries; // NativePEImportEntry

			public PEDelayImportTable ToData()
			{
				PEDelayImportTable data = new PEDelayImportTable();

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
		internal struct NativePERelocationTable : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IMAGE_BASE_RELOCATION m_Descriptor;
			public readonly ManagedArray m_Relocations; // PERelocationEntry

			public PERelocationTable ToData()
			{
				PERelocationTable data = new PERelocationTable();

				data.m_Descriptor = m_Descriptor;
				data.m_Relocations = m_Relocations.ToArray<PERelocationEntry>();

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

			public readonly ManagedArray m_Sections; // IMAGE_SECTION_HEADER

			public readonly NativePEExportTable m_Exports;

			public readonly ManagedArray m_Imports; // NativePEImportTable
			public readonly ManagedArray m_Relocations; // NativePERelocationTable
			public readonly ManagedArray m_DelayImports; // NativePEDelayImportTable

			public PEHeaders ToData()
			{
				PEHeaders data = new PEHeaders();

				data.m_Dos = m_Dos;
				data.m_NT32 = m_NT32;
				data.m_NT64 = m_NT64;

				data.m_Base = m_BaseAddress;

				data.m_Exports = m_Exports.ToData();
				data.m_Sections = m_Sections.ToArray<IMAGE_SECTION_HEADER>();

				NativePEImportTable[] imports = m_Imports.ToArray<NativePEImportTable>();

				data.m_Imports = new PEImportTable[imports.Length];
				for (int i = 0; i < imports.Length; i++)
				{
					data.m_Imports[i] = imports[i].ToData();
				}

				NativePEDelayImportTable[] delayImports = m_DelayImports.ToArray<NativePEDelayImportTable>();

				data.m_DelayImports = new PEDelayImportTable[delayImports.Length];
				for (int i = 0; i < delayImports.Length; i++)
				{
					data.m_DelayImports[i] = delayImports[i].ToData();
				}

				NativePERelocationTable[] relocations = m_Relocations.ToArray<NativePERelocationTable>();

				data.m_Relocations = new PERelocationTable[relocations.Length];
				for (int i = 0; i < relocations.Length; i++)
				{
					data.m_Relocations[i] = relocations[i].ToData();
				}

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

			public readonly ManagedArray m_Visuals;

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

			public readonly ManagedArray m_Sections;

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

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativeExecutableFunction : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly uint m_FunctionSize;
			public readonly IntPtr m_FunctionBase;

			public readonly ManagedString m_FunctionName;

			public ExecutableFunction ToData()
			{
				ExecutableFunction data = new ExecutableFunction();

				data.m_Base = m_FunctionBase;
				data.m_Size = m_FunctionSize;
				data.m_Name = m_FunctionName.ToString();

				return data;
			}

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		internal struct NativeExecutableString : IDisposable
        {
			public readonly ManagedObject m_Base;

            public readonly byte m_IsWide;
			public readonly IntPtr m_BaseAddress;

			public readonly ManagedString m_String;
			public readonly ManagedArray m_CrossReferences;

			public ExecutableString ToData()
			{
				ExecutableString data = new ExecutableString();

				data.m_Base = m_BaseAddress;
				data.m_IsWide = m_IsWide == 1 ? true : false;

				data.m_String = m_String.ToString();
				data.m_CrossReferences = m_CrossReferences.ToArray<IntPtr>();
				
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

		[DllImport("Dismantler.dll")]
		private static extern IntPtr ExecutableExplorer_GetExecutableFunctions(IntPtr instance);

		[DllImport("Dismantler.dll")]
		private static extern IntPtr ExecutableExplorer_GetPCFunction(IntPtr instance, IntPtr function);

		[DllImport("Dismantler.dll")]
		private static extern IntPtr ExecutableExplorer_GetExecutableStrings(IntPtr instance);

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

		public ExecutableFunction[] GetExecutableFunctions()
		{
			IntPtr pointer = ExecutableExplorer_GetExecutableFunctions(m_Instance);
			if (pointer == IntPtr.Zero)
			{
				return Array.Empty<ExecutableFunction>();
			}

			using (ManagedArray native = Marshal.PtrToStructure<ManagedArray>(pointer))
			{
				NativeExecutableFunction[] array = native.ToArray<NativeExecutableFunction>();
				ExecutableFunction[] functions = new ExecutableFunction[array.Length];

				for (int i = 0; i < array.Length; i++)
				{
					functions[i] = array[i].ToData();
				}
				
				return functions;
			}
		}

		public string GetPCFunction(IntPtr function)
		{
			IntPtr pointer = ExecutableExplorer_GetPCFunction(m_Instance, function);
			if (pointer == IntPtr.Zero)
			{
				return string.Empty;
			}

			using (ManagedString native = Marshal.PtrToStructure<ManagedString>(pointer))
			{
				return native.ToString();
			}
		}

		public ExecutableString[] GetExecutableStrings()
		{
			IntPtr pointer = ExecutableExplorer_GetExecutableStrings(m_Instance);
            if (pointer == IntPtr.Zero)
            {
                return Array.Empty<ExecutableString>();
            }

			using (ManagedArray native = Marshal.PtrToStructure<ManagedArray>(pointer))
			{
				NativeExecutableString[] array = native.ToArray<NativeExecutableString>();
				ExecutableString[] strings = new ExecutableString[array.Length];

				for (int i = 0; i < array.Length; i++)
				{
					strings[i] = array[i].ToData();
				}

				return strings;
			}
        }
    }
}
