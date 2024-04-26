using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sabre.Explorer.Objects
{
    internal struct ExecutableString
    {
        public bool m_IsWide;
        public IntPtr m_Base;

        public string m_String;
        public IntPtr[] m_CrossReferences;
    }
}
