using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;

namespace Sabre.Views.ByteView
{
	[Guid("69e6c853-8c18-463a-814f-6287d756aa47")]
	public class ByteViewView : ToolWindowPane
	{
		public ByteViewView() : base(null)
		{
			this.Caption = "Byte view";
			this.Content = new ByteViewControl();
		}
	}
}
