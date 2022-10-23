#ifndef TCP_FRAME_H
#define TCP_FRAME_H

//#define WIN32_LEAN_AND_MEAN  
#include "frame.h"

typedef boost::shared_ptr<TcpSocket> TcpSocketPtr;

typedef void(*TCPFrameFunEntryPoint)(void);
typedef bool(*TcpFrameFunConnect)(char* ip, char* port);
typedef bool(*TcpFrameFunBlockConnect)(char* ip, char* port, boost::shared_ptr<TcpSocket>& tcpSocket, boost::system::error_code& error);
typedef void(*TcpFrameFunSendData)(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length);
typedef void(*TcpFrameFunBlockSendData)(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length, boost::system::error_code& error);
typedef void(*TcpFrameFunClose)(boost::shared_ptr<TcpSocket> tcpSocket);

class TCPFrame
{
protected:
	TCPFrame(void);
	~TCPFrame(void);

public:
	static TCPFrame* GetInstance()
	{
		if (NULL==m_tcpFrame)
		{
			m_tcpFrame = new TCPFrame();
		}
		return m_tcpFrame;
	}


public:
	TCPFrameFunEntryPoint EntryPoint;
	TcpFrameFunConnect   Connect;
	TcpFrameFunSendData  SendData;
	bool BlockConnect(char* ip, char* port, boost::shared_ptr<TcpSocket>& tcpSocket, boost::system::error_code& error);
	void BlockSendData(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length, boost::system::error_code& error);
	void Close(boost::shared_ptr<TcpSocket> tcpSocket);
public:
	BOOL LoadTCPFrame();


private:
	static TCPFrame* m_tcpFrame;
	TcpFrameFunBlockSendData BlockSendData_;
	TcpFrameFunClose   Close_;
	TcpFrameFunBlockConnect BlockConnect_;
};

extern TCPFrame* theTCPFrame;

#endif 



