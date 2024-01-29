using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;

namespace Sabre.Views.Dismantler
{
	[Guid("63230732-614b-4268-8377-1467919ba251")]
	public class DismantlerView : ToolWindowPane
	{
		public DismantlerView() : base(null)
		{
			this.Caption = "Dismantler";
			this.Content = new DismantlerControl();
		}
	}
}
