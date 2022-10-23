#include "stdafx.h"
#include "TcpSocket.h"
#include "SQLProxyWrapper.h"
#include "CommonFunc.h"

void TcpSocket::Read()
{
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
							boost::bind(&TcpSocket::handle_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TcpSocket::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
        push_cache_data(data_, bytes_transferred);

        while (m_cache_len > 0)
        {
            uint32_t pack_len = SQLProxyWrapper::GetInstance()->TdsGetCurrentPacketLength(&m_pack_cache[0], m_cache_len);

            if (pack_len == 0)
            {
                // wrong packet size, maybe wrong packet;
                SQLProxyWrapper::GetInstance()->EndEvent(shared_from_this(), error);
                return;
            }

            if (pack_len != -1 && m_cache_len >= pack_len)
            {
                SQLProxyWrapper::GetInstance()->ReceiveDataEvent(shared_from_this(), &m_pack_cache[0], pack_len);
                remove_packet_from_cache(pack_len);
            }
            else
            {
                break;
            }
        }

        Read();
        return;
	}
	else
	{
		//events_.EndEvent(shared_from_this(), error);
        SQLProxyWrapper::GetInstance()->EndEvent(shared_from_this(), error);
	}
}

void TcpSocket::Write(BYTE* data, int length)
{
    if (data == nullptr || length == 0)
        return;

    BYTE* buf = new BYTE[length];
    memcpy(buf, data, length);

	boost::asio::async_write(socket_, boost::asio::buffer(buf, length),
							 boost::bind(&TcpSocket::handle_write, shared_from_this(), buf, boost::asio::placeholders::error));
}

void TcpSocket::BlockWrite(BYTE* data, int length, boost::system::error_code& error)
{
	boost::asio::write(socket_, boost::asio::buffer(data, length), error);
}

void TcpSocket::handle_write(BYTE* data, const boost::system::error_code& error)
{
    if (data)
        delete[] data;
    SQLProxyWrapper::GetInstance()->SendDataCompleteEvent(shared_from_this(), error);
}

void TcpSocket::Connect(const boost::asio::ip::tcp::resolver::iterator& endpoint_iterator)
{
	boost::asio::async_connect(socket_, endpoint_iterator,  
							   boost::bind(&TcpSocket::handle_connect, shared_from_this(), boost::asio::placeholders::error));
}

void TcpSocket::handle_connect(const boost::system::error_code& error)  
{  
	//events_.ConnectCompleteEvent(shared_from_this(), error);
    SQLProxyWrapper::GetInstance()->ConnectCompleteEvent(shared_from_this(), error);

	if (!error)  
	{  
		Read();
	}  
}

void TcpSocket::push_cache_data(uint8_t* data, uint32_t len)
{
    if (data == nullptr || len == 0)
        return;

    m_pack_cache.resize(m_cache_len + len);
    memcpy(&m_pack_cache[m_cache_len], data, len);
    m_cache_len += len;
}

void TcpSocket::remove_packet_from_cache(uint32_t pack_len)
{
    if (pack_len == 0)
        return;

    if (m_cache_len == pack_len)
    {
        m_cache_len = 0;
        return;
    }

    std::vector<uint8_t> tmp;
    tmp.resize(m_cache_len - pack_len);
    memcpy(&tmp[0], &m_pack_cache[pack_len], m_cache_len - pack_len);

    m_pack_cache.resize(m_cache_len - pack_len);
    memcpy(&m_pack_cache[0], &tmp[0], m_cache_len - pack_len);
    m_cache_len = m_cache_len - pack_len;
}