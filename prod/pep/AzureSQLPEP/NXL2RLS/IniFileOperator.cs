using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace NXL2RLS
{
    class IniFileOperator
    {
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        public static void IniWriteStringValue(string Section, string Key, string Value, string strFile)
        {
            WritePrivateProfileString(Section, Key, Value, strFile);
        }

        public static string IniReadStringValue(string Section, string Key, string strFile)
        {
            StringBuilder temp = new StringBuilder(500);
            int i = GetPrivateProfileString(Section, Key, "", temp, 500, strFile);
            return temp.ToString();
        }
    }
}
