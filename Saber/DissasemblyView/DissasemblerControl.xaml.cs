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

            NativeInterface.Testing();
        }
    }
}