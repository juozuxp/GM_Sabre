using Microsoft.Win32;
using Sabre.Image;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Windows;
using System.Windows.Controls;

namespace Sabre.Views.Dismantler
{
	public partial class DismantlerControl : UserControl
	{
		private byte[] m_Bytes;

		public DismantlerControl()
		{
			this.InitializeComponent();
		}

		private void LoadFile(object sender, RoutedEventArgs e)
		{
			OpenFileDialog dialog = new OpenFileDialog();

			dialog.RestoreDirectory = true;
			dialog.Filter = "Executable file|*.exe;*.dll;*.sys|All files|*.*";

			dialog.ShowDialog();

			m_Bytes = File.ReadAllBytes(dialog.FileName);

			PEDissector.Dissect(m_Bytes);
		}
	}
}