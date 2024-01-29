using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Image
{
	internal static class PEDissector
	{
		public static PEImage Dissect(byte[] bytes)
		{
			return new PEImage(new MemoryStream(bytes));
		}
	}
}
