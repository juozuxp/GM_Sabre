using Microsoft.VisualStudio.TestTools.UnitTesting;
using Sabre.Explorer;
using Sabre.Explorer.PE;
using System;
using System.Windows.Controls;

namespace Saber_Unit.Explorer.PE
{
	[TestClass]
	public class PEHeaders_Unit
	{
		[TestMethod]
		public void ToTreeView_64bit()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32.dll");

			PEHeaders? headers = explorer.GetHeaders();

			TreeView tree = new TreeView();

			headers.Value.ToTreeView(tree);

			int count = tree.Items.Count;

			Assert.AreNotEqual(tree.Items.Count, 0);
		}

		[TestMethod]
		public void ToTreeView_32bit()
		{
			ExecutableExplorer explorer = new ExecutableExplorer("kernel32_32bit.dll");

			PEHeaders? headers = explorer.GetHeaders();

			TreeView tree = new TreeView();

			headers.Value.ToTreeView(tree);

			Assert.AreNotEqual(tree.Items.Count, 0);
		}
	}
}
