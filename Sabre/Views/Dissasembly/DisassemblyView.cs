using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;

namespace Sabre.Views.Disassembly
{
	[Guid("69e6c853-8c18-463a-814f-6287d756aa47")]
	public class DisassemblyView : ToolWindowPane
	{
		public DisassemblyView() : base(null)
		{
			this.Caption = "Disassembler";
			this.Content = new DisassemblyControl();
		}
	}
}
