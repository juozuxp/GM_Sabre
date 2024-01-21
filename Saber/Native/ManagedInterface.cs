using MessagePack;
using Microsoft.VisualStudio.Text.Differencing;
using Microsoft.Win32;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Saber.Native
{
	internal class ManagedInterface
	{
		[DllImport("Dismantler.dll")]
		private static extern IntPtr Disassembler_Dissasemble(byte[] code, int length);

		public static string[] Disassemble(byte[] bytes)
		{
			IntPtr address = Disassembler_Dissasemble(bytes, bytes.Length);

			ManagedArray managed = Marshal.PtrToStructure<ManagedArray>(address);
			using (managed)
			{
				ManagedString[] strings = managed.ToArray<ManagedString>();

				string[] array = new string[strings.Length];
				for (int i = 0; i < strings.Length; i++)
				{
					array[i] = strings[i].ToString();
				}

				return array;
			}
		}
	}
}
