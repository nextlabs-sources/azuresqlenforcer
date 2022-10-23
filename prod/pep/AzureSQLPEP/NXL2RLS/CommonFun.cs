using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.IO;
namespace NXL2RLS
{
    class CommonFun
    {

        public static string GetAppDataPath()
        {
            string AppDir = System.Environment.GetFolderPath(System.Environment.SpecialFolder.LocalApplicationData);
            AppDir += "\\nextlabs\\AzureSqlEnforcer\\nxl2rls\\";
            return AppDir;
        }

        public static string ApplicationFullPath()
        {
           return Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName;
        }

        public static string ApplicationDir()
        {
            return Path.GetDirectoryName(ApplicationFullPath());
        }

        public static string ApplicationName()
        {
            return Path.GetFileName(ApplicationFullPath());
        }

        public static string GetCurrentTimeString()
        {
            DateTime dt = DateTime.Now;   
            // return string.Format("{0}{1}{2}-{3}{4}{5}", dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);
            return GetTimeString(dt);
        }

       public static string GetTimeString(DateTime dt)
        {
          return dt.ToString("yyyyMMdd-HHmmss");
        }

        public static string GetDataFolder()
        {
            string strDir = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
            strDir += "\\Nextlabs\\AzureSqlEnforcer\\NXL2RLS";

            if (!Directory.Exists(strDir))
            {
                Directory.CreateDirectory(strDir);
            }

            return strDir;
        }
    }
}
