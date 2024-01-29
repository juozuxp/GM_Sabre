using Sabre.Native.PEHeaders;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Image
{
	internal struct PEImage
	{
		public PEImage(MemoryStream stream)
		{
			m_DosHeader = new IMAGE_DOS_HEADER(stream);

			stream.Seek(m_DosHeader.e_lfanew, SeekOrigin.Begin);

			m_NTHeaders = new IMAGE_NT_HEADERS64(stream);
		}

		public IMAGE_DOS_HEADER m_DosHeader;
		public IMAGE_NT_HEADERS64 m_NTHeaders;
	}
}
