#include "TCPFrame.h"
#include "Log.h"

TCPFrame* TCPFrame::m_tcpFrame=NULL;
TCPFrame* theTCPFrame=NULL;

TCPFrame::TCPFrame(void)
{
	EntryPoint = NULL;
	Connect = NULL;
	BlockConnect_ = NULL;
	SendData = NULL;
	BlockSendData_ = NULL;
	Close_ = NULL;
}


TCPFrame::~TCPFrame(void)
{
}

BOOL TCPFrame::LoadTCPFrame()
{
	const WCHAR* szDll = L"frame.dll";

	HMODULE hFrame = ::LoadLibraryW(szDll);
	if (NULL==hFrame)
	{
		return FALSE;
	}

	EntryPoint =(TCPFrameFunEntryPoint) ::GetProcAddress(hFrame, "EntryPoint");
	Connect = (TcpFrameFunConnect)::GetProcAddress(hFrame, "Connect");
	BlockConnect_ = (TcpFrameFunBlockConnect)::GetProcAddress(hFrame, "BlockConnect");
	SendData = (TcpFrameFunSendData)::GetProcAddress(hFrame, "SendData");
	BlockSendData_ = (TcpFrameFunBlockSendData)::GetProcAddress(hFrame, "BlockSendData");
	Close_ = (TcpFrameFunClose)::GetProcAddress(hFrame, "Close");



	return EntryPoint!=NULL &&
		Connect!=NULL &&
		BlockConnect_!=NULL && 
		SendData!=NULL &&
		BlockSendData_!=NULL &&
		Close_!=NULL;
}

bool TCPFrame::BlockConnect(char* ip, char* port, boost::shared_ptr<TcpSocket>& tcpSocket, boost::system::error_code& error)
{
	try
	{
		return BlockConnect_(ip, port, tcpSocket, error);
	}
	catch (const std::exception& e)
	{
		PROXYLOG(CELOG_EMERG, "TCPFrame::BlockConnect fail [exception: %s]", e.what());
		return false;
	}
}

void TCPFrame::BlockSendData(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length, boost::system::error_code& error)
{
	if (nullptr != tcpSocket)
		BlockSendData_(tcpSocket, data, length, error);
	else
		error = boost::asio::error::make_error_code(boost::asio::error::connection_aborted);
}

void TCPFrame::Close(boost::shared_ptr<TcpSocket> tcpSocket)
{
    try
    {
        if (nullptr != tcpSocket)
            Close_(tcpSocket);
    }
    catch (std::exception e)
    {
        //PROXYLOG(CELOG_DEBUG, "TCPFrame::Close exception: %s", e.what());
    }
}
