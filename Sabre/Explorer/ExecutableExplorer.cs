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

			public void Dispose()
			{
				m_Base.Dispose();
			}
		}

		[DllImport("Dismantler.dll", CharSet = CharSet.Unicode)]
		private static extern IntPtr ExecutableExplorer_Init(string path);

		[DllImport("Dismantler.dll")]
		private static extern IntPtr ExecutableExplorer_GetHeaders(IntPtr instance);

		private readonly ManagedObject m_Instance;

		public ExecutableExplorer(string path) 
		{
			m_Instance = Marshal.PtrToStructure<ManagedObject>(ExecutableExplorer_Init(path));
		}

		~ExecutableExplorer() 
		{
			m_Instance.Dispose();
		}

		public PEHeaders GetHeaders()
		{
			using (NativeHeaders native = Marshal.PtrToStructure<NativeHeaders>(ExecutableExplorer_GetHeaders(m_Instance)))
			{
				PEHeaders headers = new PEHeaders();

				headers.m_Dos = native.m_Dos;
				headers.m_NT32 = native.m_NT32;
				headers.m_NT64 = native.m_NT64;
				headers.m_Sections = native.m_Sections.ToArray<IMAGE_SECTION_HEADER>();

				return headers;
			}

		}
	}
}
