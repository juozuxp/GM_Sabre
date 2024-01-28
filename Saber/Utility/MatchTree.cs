using System.Collections.Generic;

namespace Saber.Utility
{
	internal class MatchTree
	{
		private class Branch
		{
			private readonly List<Branch> m_Next = new List<Branch>();
			private readonly char m_Char;

			private bool m_Final;

			public Branch(char character)
			{
				m_Char = character;
			}

			public void Insert(string value, int offset)
			{
				if (value.Length <= offset)
				{
					m_Final = true;
					return;
				}

				char next = value[offset];
				foreach (Branch branch in m_Next)
				{
					if (branch.m_Char == next)
					{
						branch.Insert(value, offset + 1);
						return;
					}
				}

				Build(value, offset);
			}

			private void Build(string value, int offset)
			{
				if (value.Length <= offset)
				{
					m_Final = true;
					return;
				}

				Branch branch = new Branch(value[offset]);

				branch.Build(value, offset + 1);

				m_Next.Add(branch);
			}

			public bool Match(string value, int offset)
			{
				if (value.Length <= offset)
				{
					return false;
				}

				char next = value[offset];
				foreach (Branch branch in m_Next)
				{
					if (branch.m_Char == next)
					{
						if (branch.m_Final)
						{
							return true;
						}

						return branch.Match(value, offset + 1);
					}
				}

				return false;
			}
		}

		private Branch m_Root = new Branch(' ');

		public MatchTree(string[] matches)
		{
			foreach (string match in matches)
			{
				m_Root.Insert(match, 0);
			}
		}

		public bool Match(string value, int offset = 0)
		{
			return m_Root.Match(value, offset);
		}
	}
}
