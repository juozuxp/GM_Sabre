using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Windows.Controls;
using Sabre.WPF;
using System;
using System.Windows.Documents;
using System.Linq;
using System.Windows.Media;

namespace Saber_Unit.WPF
{
	[TestClass]
	public class BetterTextBlock_Unit
	{
		[TestMethod]
		public void DynamicText()
		{
			BetterTextBlock textblock = new BetterTextBlock();

			textblock.DynamicText = "hello world";

			Assert.AreEqual(textblock.Text, textblock.DynamicText);

			textblock.DynamicText = null;

			Assert.AreEqual(textblock.Text.Length, 0);
			Assert.AreEqual(textblock.Inlines.Count, 0);

			textblock.DynamicText = "hello world \0FFFFFFFFbut\x1 \000FF2045collored :)\x1\0FFFFFFFF\x1 and another text";

			Assert.AreEqual(textblock.Text.Length, 0);
			Assert.AreEqual(textblock.Inlines.Count, 5);

			foreach (Inline inline in textblock.Inlines)
			{
				Assert.IsInstanceOfType(inline, typeof(Run));
			}

			Run def = new Run();

			Inline[] inlines = textblock.Inlines.ToArray();

			Assert.AreEqual((inlines[0] as Run).Text, "hello world ");
			Assert.AreEqual((inlines[0] as Run).Foreground, def.Foreground);

			Assert.AreEqual((inlines[1] as Run).Text, "but");
			Assert.AreEqual(((inlines[1] as Run).Foreground as SolidColorBrush).Color, Color.FromArgb(0xFF, 0xFF, 0xFF, 0xFF));

			Assert.AreEqual((inlines[2] as Run).Text, " ");
			Assert.AreEqual((inlines[2] as Run).Foreground, def.Foreground);

			Assert.AreEqual((inlines[3] as Run).Text, "collored :)");
			Assert.AreEqual(((inlines[3] as Run).Foreground as SolidColorBrush).Color, Color.FromArgb(0x00, 0xFF, 0x20, 0x45));

			Assert.AreEqual((inlines[4] as Run).Text, " and another text");
			Assert.AreEqual((inlines[4] as Run).Foreground, def.Foreground);
		}
	}
}
