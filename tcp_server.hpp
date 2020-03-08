// TCP Socket Server
// Author: Hana
// Last update: 07/03/2020
/*---------------------------------------------------------------------------*/
#ifndef  __TCP_SERVER_HPP__
#define  __TCP_SERVER_HPP__
/*---------------------------------------------------------------------------*/
#include "tcp.hpp"			// TCPSocket class
/*---------------------------------------------------------------------------*/
class TCPSocketServer : public TCPSocket
{
public:
	TCPSocketServer(short port_, size_t maxRequests_);
	virtual ~TCPSocketServer() { };
};
/*---------------------------------------------------------------------------*/
TCPSocketServer::TCPSocketServer(short port_, size_t maxRequests_)
	: TCPSocket(port_)
{
	int status = bind(GetFD(), 
					(const sockaddr*)GetSockaddr(), *GetSocklen());
	if (EXIT_SUCCESS != status)
	{
		throw std::runtime_error("Cannot bind address");
	}
	
	status = listen(GetFD(), maxRequests_);
	if (EXIT_SUCCESS != status)
	{
		throw std::runtime_error("Cannot listen to socket");
	}
	
	fcntl(GetFD(), F_SETFL, O_NONBLOCK);
}
/*---------------------------------------------------------------------------*/
#endif // __TCP_SERVER_HPP__