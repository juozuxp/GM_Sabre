using Microsoft.Win32;
using Sabre.Explorer;
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

			if (dialog.ShowDialog() == false)
			{
				return;
			}

			ExecutableExplorer explorer = new ExecutableExplorer(dialog.FileName);

			PEHeaders headers = explorer.GetHeaders();

			headers.ToTreeView(m_HeaderView);

			m_LoadedLabel.Content = "Loaded: " + dialog.FileName;

			m_LoadedLabel.Visibility = Visibility.Visible;
			m_HeaderView.Visibility = Visibility.Visible;
		}
	}
}