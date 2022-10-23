using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.IO;

namespace SQLTestTool
{
    class CommonFun
    {
        public static string ApplicationDir()
        {
            return Path.GetDirectoryName(ApplicationFullPath());
        }

        public static string ApplicationFullPath()
        {
            return Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName;
        }
    }
}
