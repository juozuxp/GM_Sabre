using Microsoft.VisualStudio.Shell.Interop;
using Saber.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Saber.Dismantler.Visuals;

namespace Saber.Dismantler
{
	internal class Disassembler
	{
		[DllImport("Dismantler.dll")]
		private static extern IntPtr Disassembler_Init();

		[DllImport("Dismantler.dll")]
		private static extern IntPtr Disassembler_Disassemble(IntPtr instance, byte[] code, int length, IntPtr reference);

		private ManagedObject m_Instance;

		public Disassembler() 
		{
			m_Instance = Marshal.PtrToStructure<ManagedObject>(Disassembler_Init());
		}

		~Disassembler() 
		{
			m_Instance.Dispose();
		}

		public NativeVisual[] Disassemble(byte[] code, IntPtr reference)
		{
			IntPtr handle = Disassembler_Disassemble(m_Instance, code, code.Length, reference);

			using (ManagedGenericArray array = Marshal.PtrToStructure<ManagedGenericArray>(handle))
			{
				return array.ToArray<NativeVisual>();
			}
		}
	}
}
