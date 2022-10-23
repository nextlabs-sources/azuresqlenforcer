#pragma once

#include "TcpSocket.h"

using boost::asio::ip::tcp;

class FrameServer
{
public:
	FrameServer(boost::asio::io_context& io_context, tcp::endpoint endp) :io_context_(io_context), acceptor_(io_context, endp)
	{
		start_accept();
	}

	void start_accept();
	void handle_accept(boost::shared_ptr<TcpSocket> tcpSocket, const boost::system::error_code& error);

private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};

