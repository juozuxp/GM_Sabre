using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;

namespace Sabre.Views.Loader
{
	[Guid("63230732-614b-4268-8377-1467919ba251")]
	public class LoaderView : ToolWindowPane
	{
		public LoaderView() : base(null)
		{
			this.Caption = "Saber";
			this.Content = new LoaderControl();
		}
	}
}
