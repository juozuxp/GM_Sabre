using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.Win32;
using Sabre.Controller;
using Sabre.Explorer;
using Sabre.Views.ByteView;
using System.IO;
using System.Windows;
using System.Windows.Controls;

namespace Sabre.Views.Loader
{
	public partial class LoaderControl : UserControl
	{
		public LoaderControl()
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

			m_LoadedLabel.Content = "Loaded: " + dialog.FileName;

			SabreController.LoadExecutable(dialog.FileName);

			PEHeaders? headers = SabreController.GetHeaders();
			if (headers == null)
			{
				return;
			}

			headers.Value.ToTreeView(m_HeaderView);

			m_LoadedLabel.Visibility = Visibility.Visible;
			m_HeaderView.Visibility = Visibility.Visible;
		}
	}
}