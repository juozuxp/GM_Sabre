using Sabre.Native.PEHeaders;
using System;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace Sabre.Explorer
{
	internal struct PEHeaders
	{
		private static Style s_ContainerStyle = null;

		public IntPtr m_Base;

		public IMAGE_DOS_HEADER m_Dos;

		public IMAGE_NT_HEADERS32 m_NT32;
		public IMAGE_NT_HEADERS64 m_NT64;

		public PEImportTable[] m_Imports;
		public IMAGE_SECTION_HEADER[] m_Sections;

		private static ListView NewListView()
		{
			ListView view = new ListView();

			if (s_ContainerStyle == null)
			{
				Setter setter = new Setter();

				setter.Value = false;
				setter.Property = DependencyPropertyDescriptor.FromName("Focusable", typeof(UIElement), typeof(UIElement)).DependencyProperty;

				s_ContainerStyle = new Style();
				
				s_ContainerStyle.Setters.Add(setter);
			}

			GridView gridView = new GridView();
			
			GridViewColumn column = new GridViewColumn();

			column.Header = "name";
			column.DisplayMemberBinding = new Binding("m_Name");

			gridView.Columns.Add(column);

			column = new GridViewColumn();

			column.Header = "value";
			column.DisplayMemberBinding = new Binding("m_Value");

			gridView.Columns.Add(column);

			column = new GridViewColumn();

			column.Header = "info";
			column.DisplayMemberBinding = new Binding("m_Info");

			gridView.Columns.Add(column);

			view.View = gridView;
			view.ItemContainerStyle = s_ContainerStyle;

			return view;
		}

		private TreeViewItem DosToView()
		{
			TreeViewItem item = new TreeViewItem();

			item.Header = "dos header";

			ListView view = NewListView();

			view.Items.Add(new { m_Name = "Magic number", m_Value = m_Dos.e_magic.ToString("X4"), m_Info = Encoding.UTF8.GetString(BitConverter.GetBytes(m_Dos.e_magic), 0, 2) });
			view.Items.Add(new { m_Name = "Bytes on last page of file", m_Value = m_Dos.e_cblp.ToString("X4") });
			view.Items.Add(new { m_Name = "Pages in file", m_Value = m_Dos.e_cp.ToString("X4") });
			view.Items.Add(new { m_Name = "Relocations", m_Value = m_Dos.e_crlc.ToString("X4") });
			view.Items.Add(new { m_Name = "Size of header in paragraphs", m_Value = m_Dos.e_cparhdr.ToString("X4") });
			view.Items.Add(new { m_Name = "Minimum extra paragraphs needed", m_Value = m_Dos.e_minalloc.ToString("X4") });
			view.Items.Add(new { m_Name = "Maximum extra paragraphs needed", m_Value = m_Dos.e_maxalloc.ToString("X4") });
			view.Items.Add(new { m_Name = "Initial (relative) SS value", m_Value = m_Dos.e_ss.ToString("X4") });
			view.Items.Add(new { m_Name = "Initial SP value", m_Value = m_Dos.e_sp.ToString("X4") });
			view.Items.Add(new { m_Name = "Checksum", m_Value = m_Dos.e_csum.ToString("X4") });
			view.Items.Add(new { m_Name = "Initial IP value", m_Value = m_Dos.e_ip.ToString("X4") });
			view.Items.Add(new { m_Name = "Initial (relative) CS value", m_Value = m_Dos.e_cs.ToString("X4") });
			view.Items.Add(new { m_Name = "File address of relocation table", m_Value = m_Dos.e_lfarlc.ToString("X4") });
			view.Items.Add(new { m_Name = "Overlay number", m_Value = m_Dos.e_ovno.ToString("X4") });
			view.Items.Add(new { m_Name = "Reserved words", m_Value = $"{{{string.Join(", ", m_Dos.e_res.Select(x => x.ToString("X4")))}}}" });
			view.Items.Add(new { m_Name = "OEM identifier", m_Value = m_Dos.e_oemid.ToString("X4") });
			view.Items.Add(new { m_Name = "OEM information", m_Value = m_Dos.e_oeminfo.ToString("X4") });
			view.Items.Add(new { m_Name = "Reserved words", m_Value = $"{{{string.Join(", ", m_Dos.e_res2.Select(x => x.ToString("X4")))}}}" });
			view.Items.Add(new { m_Name = "File address of new exe header", m_Value = m_Dos.e_lfanew.ToString("X8"), m_Info = m_Dos.e_lfanew != 0 ? (m_Base + m_Dos.e_lfanew).ToString("X16") : 0.ToString("X16") });

			item.Items.Add(view);

			return item;
		}

		private TreeViewItem FileToView()
		{
			TreeViewItem item = new TreeViewItem();

			item.Header = "file header";

			ListView view = NewListView();

			view.Items.Add(new { m_Name = "Machine", m_Value = m_NT32.FileHeader.Machine.ToString("X4") });
			view.Items.Add(new { m_Name = "Number Of Sections", m_Value = m_NT32.FileHeader.NumberOfSections.ToString("X4") });
			view.Items.Add(new { m_Name = "Time Date Stamp", m_Value = m_NT32.FileHeader.TimeDateStamp.ToString("X8"), m_Info = DateTimeOffset.FromUnixTimeSeconds(m_NT32.FileHeader.TimeDateStamp).ToString("F") });
			view.Items.Add(new { m_Name = "Pointer To Symbol Table", m_Value = m_NT32.FileHeader.PointerToSymbolTable.ToString("X8"), m_Info = m_NT32.FileHeader.PointerToSymbolTable != 0 ? (m_Base + (int)m_NT32.FileHeader.PointerToSymbolTable).ToString("X16") : 0.ToString("X16") });
			view.Items.Add(new { m_Name = "Number Of Symbols", m_Value = m_NT32.FileHeader.NumberOfSymbols.ToString("X8") });
			view.Items.Add(new { m_Name = "Size Of Optional Header", m_Value = m_NT32.FileHeader.SizeOfOptionalHeader.ToString("X4") });
			view.Items.Add(new { m_Name = "Characteristics", m_Value = m_NT32.FileHeader.Characteristics.ToString("X4") });

			item.Items.Add(view);

			return item;
		}

		private TreeViewItem OptionalToView()
		{
			TreeViewItem item = new TreeViewItem();

			item.Header = "optional header";

			IMAGE_DATA_DIRECTORY[] directories;
			if (m_NT32.FileHeader.SizeOfOptionalHeader == Marshal.SizeOf<IMAGE_OPTIONAL_HEADER32>())
			{
				ListView view = NewListView();

				view.Items.Add(new { m_Name = "Magic", m_Value = m_NT32.OptionalHeader.Magic.ToString("X4") });
				view.Items.Add(new { m_Name = "Major Linker Version", m_Value = m_NT32.OptionalHeader.MajorLinkerVersion.ToString("X2") });
				view.Items.Add(new { m_Name = "Minor Linker Version", m_Value = m_NT32.OptionalHeader.MinorLinkerVersion.ToString("X2") });
				view.Items.Add(new { m_Name = "Size Of Code", m_Value = m_NT32.OptionalHeader.SizeOfCode.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Initialized Data", m_Value = m_NT32.OptionalHeader.SizeOfInitializedData.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Uninitialized Data", m_Value = m_NT32.OptionalHeader.SizeOfUninitializedData.ToString("X8") });
				view.Items.Add(new { m_Name = "Address Of Entry Point", m_Value = m_NT32.OptionalHeader.AddressOfEntryPoint.ToString("X8"), m_Info = m_NT32.OptionalHeader.AddressOfEntryPoint != 0 ? (m_Base + (int)m_NT32.OptionalHeader.AddressOfEntryPoint).ToString("X16") : 0.ToString("X16") });
				view.Items.Add(new { m_Name = "Base Of Code", m_Value = m_NT32.OptionalHeader.BaseOfCode.ToString("X8"), m_Info = m_NT32.OptionalHeader.BaseOfCode != 0 ? (m_Base + (int)m_NT32.OptionalHeader.BaseOfCode).ToString("X16") : 0.ToString("X16") });
				view.Items.Add(new { m_Name = "Base Of Data", m_Value = m_NT32.OptionalHeader.BaseOfData.ToString("X8"), m_Info = m_NT32.OptionalHeader.BaseOfData != 0 ? (m_Base + (int)m_NT32.OptionalHeader.BaseOfData).ToString("X16") : 0.ToString("X16") });
				view.Items.Add(new { m_Name = "Image Base", m_Value = m_NT32.OptionalHeader.ImageBase.ToString("X16") });
				view.Items.Add(new { m_Name = "Section Alignment", m_Value = m_NT32.OptionalHeader.SectionAlignment.ToString("X8") });
				view.Items.Add(new { m_Name = "File Alignment", m_Value = m_NT32.OptionalHeader.FileAlignment.ToString("X8") });
				view.Items.Add(new { m_Name = "Major Operating System Version", m_Value = m_NT32.OptionalHeader.MajorOperatingSystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Minor Operating System Version", m_Value = m_NT32.OptionalHeader.MinorOperatingSystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Major Image Version", m_Value = m_NT32.OptionalHeader.MajorImageVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Minor Image Version", m_Value = m_NT32.OptionalHeader.MinorImageVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Major Subsystem Version", m_Value = m_NT32.OptionalHeader.MajorSubsystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Minor Subsystem Version", m_Value = m_NT32.OptionalHeader.MinorSubsystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Win32 Version Value", m_Value = m_NT32.OptionalHeader.Win32VersionValue.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Image", m_Value = m_NT32.OptionalHeader.SizeOfImage.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Headers", m_Value = m_NT32.OptionalHeader.SizeOfHeaders.ToString("X8") });
				view.Items.Add(new { m_Name = "Check Sum", m_Value = m_NT32.OptionalHeader.CheckSum.ToString("X8") });
				view.Items.Add(new { m_Name = "Subsystem", m_Value = m_NT32.OptionalHeader.Subsystem.ToString("X4") });
				view.Items.Add(new { m_Name = "Dll Characteristics", m_Value = m_NT32.OptionalHeader.DllCharacteristics.ToString("X4") });
				view.Items.Add(new { m_Name = "Size Of Stack Reserve", m_Value = m_NT32.OptionalHeader.SizeOfStackReserve.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Stack Commit", m_Value = m_NT32.OptionalHeader.SizeOfStackCommit.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Heap Reserve", m_Value = m_NT32.OptionalHeader.SizeOfHeapReserve.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Heap Commit", m_Value = m_NT32.OptionalHeader.SizeOfHeapCommit.ToString("X8") });
				view.Items.Add(new { m_Name = "Loader Flags", m_Value = m_NT32.OptionalHeader.LoaderFlags.ToString("X8") });
				view.Items.Add(new { m_Name = "Number Of Rva And Sizes", m_Value = m_NT32.OptionalHeader.NumberOfRvaAndSizes.ToString("X8") });

				item.Items.Add(view);

				directories = m_NT32.OptionalHeader.DataDirectory;
			}
			else
			{
				ListView view = NewListView();

				view.Items.Add(new { m_Name = "Magic", m_Value = m_NT64.OptionalHeader.Magic.ToString("X4") });
				view.Items.Add(new { m_Name = "Major Linker Version", m_Value = m_NT64.OptionalHeader.MajorLinkerVersion.ToString("X2") });
				view.Items.Add(new { m_Name = "Minor Linker Version", m_Value = m_NT64.OptionalHeader.MinorLinkerVersion.ToString("X2") });
				view.Items.Add(new { m_Name = "Size Of Code", m_Value = m_NT64.OptionalHeader.SizeOfCode.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Initialized Data", m_Value = m_NT64.OptionalHeader.SizeOfInitializedData.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Uninitialized Data", m_Value = m_NT64.OptionalHeader.SizeOfUninitializedData.ToString("X8") });
				view.Items.Add(new { m_Name = "Address Of Entry Point", m_Value = m_NT64.OptionalHeader.AddressOfEntryPoint.ToString("X8"), m_Info = m_NT64.OptionalHeader.AddressOfEntryPoint != 0 ? (m_Base + (int)m_NT64.OptionalHeader.AddressOfEntryPoint).ToString("X16") : 0.ToString("X16") });
				view.Items.Add(new { m_Name = "Base Of Code", m_Value = m_NT64.OptionalHeader.BaseOfCode.ToString("X8"), m_Info = m_NT64.OptionalHeader.BaseOfCode != 0 ? (m_Base + (int)m_NT64.OptionalHeader.BaseOfCode).ToString("X16") : 0.ToString("X16") });
				view.Items.Add(new { m_Name = "Image Base", m_Value = m_NT64.OptionalHeader.ImageBase.ToString("X16") });
				view.Items.Add(new { m_Name = "Section Alignment", m_Value = m_NT64.OptionalHeader.SectionAlignment.ToString("X8") });
				view.Items.Add(new { m_Name = "File Alignment", m_Value = m_NT64.OptionalHeader.FileAlignment.ToString("X8") });
				view.Items.Add(new { m_Name = "Major Operating System Version", m_Value = m_NT64.OptionalHeader.MajorOperatingSystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Minor Operating System Version", m_Value = m_NT64.OptionalHeader.MinorOperatingSystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Major Image Version", m_Value = m_NT64.OptionalHeader.MajorImageVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Minor Image Version", m_Value = m_NT64.OptionalHeader.MinorImageVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Major Subsystem Version", m_Value = m_NT64.OptionalHeader.MajorSubsystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Minor Subsystem Version", m_Value = m_NT64.OptionalHeader.MinorSubsystemVersion.ToString("X4") });
				view.Items.Add(new { m_Name = "Win32 Version Value", m_Value = m_NT64.OptionalHeader.Win32VersionValue.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Image", m_Value = m_NT64.OptionalHeader.SizeOfImage.ToString("X8") });
				view.Items.Add(new { m_Name = "Size Of Headers", m_Value = m_NT64.OptionalHeader.SizeOfHeaders.ToString("X8") });
				view.Items.Add(new { m_Name = "Check Sum", m_Value = m_NT64.OptionalHeader.CheckSum.ToString("X8") });
				view.Items.Add(new { m_Name = "Subsystem", m_Value = m_NT64.OptionalHeader.Subsystem.ToString("X4") });
				view.Items.Add(new { m_Name = "Dll Characteristics", m_Value = m_NT64.OptionalHeader.DllCharacteristics.ToString("X4") });
				view.Items.Add(new { m_Name = "Size Of Stack Reserve", m_Value = m_NT64.OptionalHeader.SizeOfStackReserve.ToString("X16") });
				view.Items.Add(new { m_Name = "Size Of Stack Commit", m_Value = m_NT64.OptionalHeader.SizeOfStackCommit.ToString("X16") });
				view.Items.Add(new { m_Name = "Size Of Heap Reserve", m_Value = m_NT64.OptionalHeader.SizeOfHeapReserve.ToString("X16") });
				view.Items.Add(new { m_Name = "Size Of Heap Commit", m_Value = m_NT64.OptionalHeader.SizeOfHeapCommit.ToString("X16") });
				view.Items.Add(new { m_Name = "Loader Flags", m_Value = m_NT64.OptionalHeader.LoaderFlags.ToString("X8") });
				view.Items.Add(new { m_Name = "Number Of Rva And Sizes", m_Value = m_NT64.OptionalHeader.NumberOfRvaAndSizes.ToString("X8") });

				item.Items.Add(view);

				directories = m_NT64.OptionalHeader.DataDirectory;
			}

			TreeViewItem directoryItem = new TreeViewItem();
			IMAGE_DATA_DIRECTORY directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_EXPORT];

			directoryItem.Header = "export table";

			ListView directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_IMPORT];

			directoryItem.Header = "import table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_RESOURCE];

			directoryItem.Header = "resource table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_EXCEPTION];

			directoryItem.Header = "exception table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_SECURITY];

			directoryItem.Header = "certificate table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_BASERELOC];

			directoryItem.Header = "base relocation table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_DEBUG];

			directoryItem.Header = "debug";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_ARCHITECTURE];

			directoryItem.Header = "architecture";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_GLOBALPTR];

			directoryItem.Header = "global ptr";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_TLS];

			directoryItem.Header = "thread local storage table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];

			directoryItem.Header = "load config table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT];

			directoryItem.Header = "bound import";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_IAT];

			directoryItem.Header = "import address table";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];

			directoryItem.Header = "delay import descriptor";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			directoryItem = new TreeViewItem();
			directory = directories[IMAGE_OPTIONAL_HEADER32.IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];

			directoryItem.Header = "clr runtime header";

			directoryView = NewListView();

			directoryView.Items.Add(new { m_Name = "Virtual Address", m_Value = directory.VirtualAddress.ToString("X8"), m_Info = directory.VirtualAddress != 0 ? (m_Base + (int)directory.VirtualAddress).ToString("X16") : 0.ToString("X16") });
			directoryView.Items.Add(new { m_Name = "Size", m_Value = directory.Size.ToString("X8") });

			directoryItem.Items.Add(directoryView);

			item.Items.Add(directoryItem);

			return item;
		}

		private TreeViewItem NTToView()
		{
			TreeViewItem item = new TreeViewItem();

			item.Header = "nt headers";

			ListView view = NewListView();

			view.Items.Add(new { m_Name = "Signature", m_Value = m_NT32.Signature.ToString("X8"), m_Info = Encoding.UTF8.GetString(BitConverter.GetBytes(m_NT32.Signature), 0, 4) });

			item.Items.Add(view);
			item.Items.Add(FileToView());
			item.Items.Add(OptionalToView());

			return item;
		}

		private TreeViewItem SectionsToView()
		{
			TreeViewItem item = new TreeViewItem();

			item.Header = "sections";

			foreach (IMAGE_SECTION_HEADER section in m_Sections)
			{
				TreeViewItem sectionItem = new TreeViewItem();

				sectionItem.Header = Encoding.UTF8.GetString(section.Name, 0, 8);

				ListView view = NewListView();

				view.Items.Add(new { m_Name = "Name", m_Value = $"{{{string.Join(", ", section.Name.Select(x => x.ToString("X2")))}}}", m_Info = Encoding.UTF8.GetString(section.Name, 0, 8) });
				view.Items.Add(new { m_Name = "Virtual Size", m_Value = section.VirtualSize.ToString("X8") });

				view.Items.Add(new { m_Name = "Virtual Address", m_Value = section.VirtualAddress.ToString("X8"), m_Info = section.VirtualAddress != 0 ? (m_Base + (int)section.VirtualAddress).ToString("X16") : 0.ToString("X16") });

				view.Items.Add(new { m_Name = "Size Of Raw Data", m_Value = section.SizeOfRawData.ToString("X8") });
				view.Items.Add(new { m_Name = "Pointer To Raw Data", m_Value = section.PointerToRawData.ToString("X8") });

				view.Items.Add(new { m_Name = "Pointer To Relocations", m_Value = section.PointerToRelocations.ToString("X8"), m_Info = section.PointerToRelocations != 0 ? (m_Base + (int)section.PointerToRelocations).ToString("X16") : 0.ToString("X16") });
				view.Items.Add(new { m_Name = "Pointer To Line Numbers", m_Value = section.PointerToLinenumbers.ToString("X8"), m_Info = section.PointerToLinenumbers != 0 ? (m_Base + (int)section.PointerToLinenumbers).ToString("X16") : 0.ToString("X16") });

				view.Items.Add(new { m_Name = "Number Of Relocations", m_Value = section.NumberOfRelocations.ToString("X4") });
				view.Items.Add(new { m_Name = "Number Of Line Numbers", m_Value = section.NumberOfLinenumbers.ToString("X4") });
				view.Items.Add(new { m_Name = "Characteristics", m_Value = section.Characteristics.ToString("X8") });

				sectionItem.Items.Add(view);
				item.Items.Add(sectionItem);
			}

			return item;
		}

		private TreeViewItem ImportsToView()
		{
			TreeViewItem item = new TreeViewItem();

			item.Header = "imports";

			foreach (PEImportTable table in m_Imports)
			{
				TreeViewItem tableItem = new TreeViewItem();

				tableItem.Header = table.m_Name;

				ListView descriptorView = NewListView();

				descriptorView.Items.Add(new { m_Name = "RVA of the import lookup table", m_Value = table.m_Descriptor.OriginalFirstThunk.ToString("X8"), m_Info = table.m_Descriptor.OriginalFirstThunk != 0 ? (m_Base + (int)table.m_Descriptor.OriginalFirstThunk).ToString("X16") : 0.ToString("X16") });
				descriptorView.Items.Add(new { m_Name = "Time date stamp", m_Value = table.m_Descriptor.TimeDateStamp.ToString("X8"), m_Info = DateTimeOffset.FromUnixTimeSeconds(table.m_Descriptor.TimeDateStamp).ToString("F") });
				descriptorView.Items.Add(new { m_Name = "Forwarder Chain", m_Value = table.m_Descriptor.ForwarderChain.ToString("X8") });
				descriptorView.Items.Add(new { m_Name = "Name", m_Value = table.m_Descriptor.Name.ToString("X8"), m_Info = table.m_Name });
				descriptorView.Items.Add(new { m_Name = "RVA of the import address table", m_Value = table.m_Descriptor.FirstThunk.ToString("X8"), m_Info = table.m_Descriptor.FirstThunk != 0 ? (m_Base + (int)table.m_Descriptor.FirstThunk).ToString("X16") : 0.ToString("X16") });

				tableItem.Items.Add(descriptorView);

				foreach (PEImportEntry import in table.m_Entries)
				{
					TreeViewItem importItem = new TreeViewItem();

					importItem.Header = import.m_Name.Length != 0 ? import.m_Name : import.m_Ordinal.ToString("X4");

					ListView importView = NewListView();

					if (import.m_Name.Length == 0)
					{
						importView.Items.Add(new { m_Name = "Ordinal", m_Value = import.m_Ordinal.ToString("X4") });
					}
					else
					{
						importView.Items.Add(new { m_Name = "Name", m_Value = import.m_Name });
						importView.Items.Add(new { m_Name = "Hint", m_Value = import.m_Ordinal.ToString("X4") });
					}

					importView.Items.Add(new { m_Name = "RVA of the import address", m_Value = import.m_FirstThunk.ToString("X8"), m_Info = import.m_FirstThunk != 0 ? (m_Base + (int)import.m_FirstThunk).ToString("X16") : 0.ToString("X16") });
					importView.Items.Add(new { m_Name = "RVA of the import lookup", m_Value = import.m_OriginalFirstThunk.ToString("X8"), m_Info = import.m_OriginalFirstThunk != 0 ? (m_Base + (int)import.m_OriginalFirstThunk).ToString("X16") : 0.ToString("X16") });

					importItem.Items.Add(importView);
					tableItem.Items.Add(importItem);
				}

				item.Items.Add(tableItem);
			}

			return item;
		}

		public void ToTreeView(TreeView view)
		{
			view.Items.Clear();

			view.Items.Add(DosToView());
			view.Items.Add(NTToView());
			view.Items.Add(SectionsToView());
			view.Items.Add(ImportsToView());
		}
	}
}
