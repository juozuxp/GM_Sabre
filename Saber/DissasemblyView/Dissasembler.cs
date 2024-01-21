using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;

namespace Saber.DissasemblyView
{
	/// <summary>
	/// This class implements the tool window exposed by this package and hosts a user control.
	/// </summary>
	/// <remarks>
	/// In Visual Studio tool windows are composed of a frame (implemented by the shell) and a pane,
	/// usually implemented by the package implementer.
	/// <para>
	/// This class derives from the ToolWindowPane class provided from the MPF in order to use its
	/// implementation of the IVsUIElementPane interface.
	/// </para>
	/// </remarks>
	[Guid("69e6c853-8c18-463a-814f-6287d756aa47")]
	public class Dissasembler : ToolWindowPane
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="Dissasembler"/> class.
		/// </summary>
		public Dissasembler() : base(null)
		{
			this.Caption = "Dissasembler";

			// This is the user control hosted by the tool window; Note that, even if this class implements IDisposable,
			// we are not calling Dispose on this object. This is because ToolWindowPane calls Dispose on
			// the object returned by the Content property.
			this.Content = new DissasemblerControl();
		}
	}
}
