#include "SQLServerInstanceHelper.h"
#include "UdpSocket.h"
#include "logger_class.h"
#include <stdint.h>

#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>

size_t UdpSend(boost::asio::mutable_buffer& recv_buff, const char* ep, const char* port, uint8_t* data, uint32_t len, boost::system::error_code& ec)
{
    if (nullptr == ep || 0 == port || nullptr == data || len == 0)
        return 0;

    boost::asio::io_service query_service;
    boost::asio::ip::udp::resolver resolver(query_service);
    boost::asio::ip::udp::resolver::query query(ep, port);
    boost::asio::ip::udp::resolver::iterator remote_ep_iter = resolver.resolve(query, ec);
    boost::asio::ip::udp::resolver::iterator iter_end;

    boost::asio::io_service io_service;
    boost::asio::ip::udp::endpoint local_endp(boost::asio::ip::udp::v4(), 0);

    UdpSocket udpsock(io_service, local_endp);

    for (;remote_ep_iter != iter_end; remote_ep_iter++)
    {
        udpsock.Send(boost::asio::buffer(data, len), remote_ep_iter->endpoint(), ec);
        if (!ec)
        {
            size_t recv_len = udpsock.Receive(boost::asio::buffer(recv_buff), boost::posix_time::seconds(10), ec);
            if (!ec)
            {
                return recv_len;
            }
        }
    }

    return 0;
}

uint16_t QueryInstancePort(const std::string& inst, const std::string& host, boost::system::error_code& ec)
{
    uint16_t port = 0;
    std::vector<std::string> params;
    boost::array<uint8_t, 2048> query_buff;
    uint8_t* query_data = new uint8_t[inst.length() + 1];
    query_data[0] = 0x04;
    memcpy(query_data + 1, inst.c_str(), inst.length());
    
    try
    {
        size_t len = UdpSend(boost::asio::buffer(query_buff), host.c_str(), "1434", query_data, inst.length() + 1, ec);
        if (!ec && len > 3)
        {
            uint8_t* p = query_buff.data();
            if (p[0] == 0x05)
            {
                std::string str;
                str.append((char*)(p + 3), (char*)(p + len));

                boost::split(params, str, boost::algorithm::is_any_of(";"), boost::token_compress_on);

                for (size_t i = 0; i < params.size(); i++)
                {
                    if (_stricmp(params[i].c_str(), "tcp") == 0) {
                        port = (uint16_t)(uint32_t)atoi(params[i+1].c_str());
                    }
                }
            }
        }
    }
    catch (std::exception e)
    {
        ;
    }

    delete[] query_data;
    return port;
}

bool TryGetInstancePort(const char* inst_name, const char* host)
{
    if (nullptr == inst_name || nullptr == host || !inst_name[0] || !host[0])
        return false;

    boost::system::error_code errcode;
    uint16_t port = QueryInstancePort(inst_name, host, errcode);
    if (!errcode && port != 0)
        return true;
    
    daebootstrap::Logger::Instance().Error("Can not get port of the instance name %s", inst_name);
    return false;
}