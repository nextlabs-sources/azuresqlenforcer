#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#pragma warning(push,0)
#pragma warning(disable:4702 26812 6388 26451 26498 26495 6387 6258 6001 6255 6031 26439 5204 5219 5045 4355 4548 28251)
#include <boost/asio/io_service.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#pragma warning(pop)
#include <map>

using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http; // from <boost/beast/http.hpp>

class NXLHttpClient
{

public:
	~NXLHttpClient();
	NXLHttpClient(const char* szServer, const char* szPort);
	
	bool Request(http::request<http::string_body>& refReq,
		boost::beast::flat_buffer& refBuf,
		http::response<http::string_body>& refResp);

	void Reset();

private:
	bool IsHttps();
	bool ConnectToServer();
	bool ConnectToHttpsServer();
	bool ConnectToHttpServer();
	void Clear();

private:
	std::string m_strServer;
	std::string m_strPort;
	std::string m_strHost;


	//about socket
	boost::asio::io_service m_ioService;

	tcp::socket*  m_pHttpSocket;

	ssl::context m_SSLCtx;
	ssl::stream<tcp::socket>* m_pHttpsStream;
   
	bool m_bConnected;

};



#endif 
