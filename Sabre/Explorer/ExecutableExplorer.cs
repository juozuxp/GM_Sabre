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
		[StructLayout(LayoutKind.Sequential)]
		internal struct NativeHeaders : IDisposable
		{
			private readonly ManagedObject m_Base;

			public readonly IMAGE_DOS_HEADER m_Dos;
			public readonly IMAGE_NT_HEADERS32 m_NT32;
			public readonly IMAGE_NT_HEADERS64 m_NT64;

			public readonly ManagedGenericArray m_Sections; // IMAGE_SECTION_HEADER

			public PEHeaders ToData()
			{
				PEHeaders data = new PEHeaders();

				data.m_Dos = m_Dos;
				data.m_NT32 = m_NT32;
				data.m_NT64 = m_NT64;
				data.m_Sections = m_Sections.ToArray<IMAGE_SECTION_HEADER>();

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

			public readonly uint m_SizeOfView; // size of the data

			public ExecutableView ToData()
			{
				ExecutableView data = new ExecutableView();

				data.m_DataAddress = m_DataAddress;
				data.m_SizeOfView = m_SizeOfView;
				data.m_BaseAddress = m_BaseAddress;

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
