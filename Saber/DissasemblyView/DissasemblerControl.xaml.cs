using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using Saber.Native;

namespace Saber.DissasemblyView
{
    /// <summary>
    /// Interaction logic for DissasemblerControl.
    /// </summary>
    public partial class DissasemblerControl : UserControl
    {
        ScrollViewer m_DissasemblyViewer;

        /// <summary>
        /// Initializes a new instance of the <see cref="DissasemblerControl"/> class.
        /// </summary>
        public DissasemblerControl()
        {
            this.InitializeComponent();

            m_DissasemblyViewer = (ScrollViewer)FindName("dism_view");

            NativeInterface native = new NativeInterface();

            native.Dissasemble(new byte[] { 0x48, 0x8B, 0x44, 0xD8, 0x50 });
		}
    }
}