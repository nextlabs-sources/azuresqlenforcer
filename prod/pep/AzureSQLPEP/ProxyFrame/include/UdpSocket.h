#pragma once

#ifndef __UDP_SOCKET_H__s
#define __UDP_SOCKET_H__s

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

class UdpSocket
{
public:
    UdpSocket(boost::asio::io_service& serv, boost::asio::ip::udp::endpoint& ep)
        :m_socket(serv, ep)
        ,m_deadline(serv)
        ,m_pIoService(&serv)
    {
        m_deadline.expires_at(boost::posix_time::pos_infin);

        CheckDeadline();
    }

    ~UdpSocket()
    {
        try
        {
            m_socket.close();
        }
        catch (...)
        {

        }
    }

    size_t Send(const boost::asio::mutable_buffer& buffer, boost::asio::ip::udp::endpoint& ep, boost::system::error_code& ec);

    size_t Receive(const boost::asio::mutable_buffer& buffer, boost::posix_time::time_duration timeout, boost::system::error_code& ec);

private:
    void CheckDeadline();

    void HandleReceive(const boost::system::error_code& ec, std::size_t length, boost::system::error_code* out_ec, std::size_t* out_length);
    
private:
    boost::asio::io_service*     m_pIoService;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::deadline_timer  m_deadline;
};

#endif