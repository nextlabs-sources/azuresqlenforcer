#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <vector>

class TcpSocket : public boost::enable_shared_from_this<TcpSocket>
{
public:
	TcpSocket(boost::asio::io_context& io_context)
		: socket_(io_context)
		, m_data(nullptr)
		, m_outgoing(false)
        , m_cache_len(0)
	{
	}

	boost::asio::ip::tcp::socket& socket()
	{
		return socket_;
	}

	void Read();
	void Write(BYTE* data, int length);
	void BlockWrite(BYTE* data, int length, boost::system::error_code& error);

	void Connect(const boost::asio::ip::tcp::resolver::iterator& endpoint_iterator);

	void SetData(void* data) { m_data = data; }
	void* GetData() const { return m_data; }

	void SetOutgoing(bool value) { m_outgoing = value; }
	bool IsOutgoing() const { return m_outgoing; }
private:
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(BYTE* data, const boost::system::error_code& error);

	void handle_connect(const boost::system::error_code& error);

    void push_cache_data(uint8_t* data, uint32_t len);
    void remove_packet_from_cache(uint32_t pack_len);

private:
	boost::asio::ip::tcp::socket socket_;
	void* m_data;
	//Is this a active socket to the Server (the back-end server) from the local (the Proxy itself)
	//Active sockets may be created by accept() or connect(), named, incoming and outgoing connections.
	//https://serverfault.com/questions/443038/what-does-incoming-and-outgoing-traffic-mean
	//https://stackoverflow.com/questions/4696812/passive-and-active-sockets
	//http://man7.org/linux/man-pages/man2/listen.2.html
	bool m_outgoing;

	enum { max_length = 4096 };
	BYTE data_[max_length];

    std::vector<uint8_t> m_pack_cache;
    uint32_t m_cache_len;
};

