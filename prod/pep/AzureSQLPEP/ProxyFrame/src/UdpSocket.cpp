#include "stdafx.h"
#include "UdpSocket.h"

size_t UdpSocket::Send(const boost::asio::mutable_buffer& buffer, boost::asio::ip::udp::endpoint& ep, boost::system::error_code& ec)
{
    return m_socket.send_to(boost::asio::buffer(buffer), ep, 0, ec);
}

size_t UdpSocket::Receive(const boost::asio::mutable_buffer& buffer, boost::posix_time::time_duration timeout, boost::system::error_code& ec)
{
    // Set a deadline for the asynchronous operation.
    m_deadline.expires_from_now(timeout);

    // Set up the variables that receive the result of the asynchronous
    // operation. The error code is set to would_block to signal that the
    // operation is incomplete. Asio guarantees that its asynchronous
    // operations will never fail with would_block, so any other value in
    // ec indicates completion.
    ec = boost::asio::error::would_block;
    std::size_t length = 0;

    // Start the asynchronous operation itself. The handle_receive function
    // used as a callback will update the ec and length variables.
    m_socket.async_receive(boost::asio::buffer(buffer),
        boost::bind(&UdpSocket::HandleReceive, this, _1, _2, &ec, &length));

    // Block until the asynchronous operation has completed.
    do {
        m_pIoService->run_one();
    } while (ec == boost::asio::error::would_block);

    return length;
}

void UdpSocket::CheckDeadline()
{
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (m_deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now())
    {
        // The deadline has passed. The outstanding asynchronous operation needs
        // to be cancelled so that the blocked receive() function will return.
        //
        // Please note that cancel() has portability issues on some versions of
        // Microsoft Windows, and it may be necessary to use close() instead.
        // Consult the documentation for cancel() for further information.
        m_socket.cancel();

        // There is no longer an active deadline. The expiry is set to positive
        // infinity so that the actor takes no action until a new deadline is set.
        m_deadline.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    m_deadline.async_wait(boost::bind(&UdpSocket::CheckDeadline, this));
}

void UdpSocket::HandleReceive(const boost::system::error_code& ec, std::size_t length, boost::system::error_code* out_ec, std::size_t* out_length)
{
    if (out_ec)
        *out_ec = ec;
    if (out_length)
        *out_length = length;
}