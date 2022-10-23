using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SQLTestTool
{
    public class MessageEventArgs : EventArgs
    {
        public String m_Format;
        public object[] m_args;
        public MessageEventArgs(string fmt, params object[] args)
        {
            m_Format = fmt;
            m_args = args;
        }
    }

    class Log
    {
        private static TextBox m_logBox;

        public delegate void MessageHandler(MessageEventArgs e);

        public static void Init(TextBox logBox)
        {
            m_logBox = logBox;
        }


        public static void Message(MessageEventArgs e)
        {
            m_logBox.Text += string.Format(e.m_Format, e.m_args);
        }

        public  static void WriteLog(string strFmt, params object[] args)
        {
            try
            {
                MessageHandler handler = new MessageHandler(Message);
                MessageEventArgs argsFmt = new MessageEventArgs(strFmt, args);
                m_logBox.Invoke(handler, new object[] { argsFmt });
            }
            catch(Exception)
            {
                m_logBox.Text += string.Format(strFmt, args);
            }
           
        }



    }
}
