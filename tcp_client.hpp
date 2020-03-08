// TCP Socket Client
// Author: Hana
// Last update: 07/03/2020
/*---------------------------------------------------------------------------*/
#ifndef  __TCP_CLIENT_HPP__
#define  __TCP_CLIENT_HPP__
/*---------------------------------------------------------------------------*/
#include "tcp.hpp"			// TCPSocket class
/*---------------------------------------------------------------------------*/
class TCPSocketClient : public TCPSocket
{
public:
	TCPSocketClient(short port_);
	virtual ~TCPSocketClient() { };
};
/*---------------------------------------------------------------------------*/
TCPSocketClient::TCPSocketClient(short port_)
	: TCPSocket(port_)
{
	int status = connect(GetFD(), 
					(struct sockaddr*)GetSockaddr(), *GetSocklen());
	
	if ((EXIT_SUCCESS != status) && (errno != EINPROGRESS))
	{
		perror("Cannot connect to socket");
		throw std::runtime_error("Cannot connect to socket");
	}
}
/*---------------------------------------------------------------------------*/
#endif // __TCP_CLIENT_HPP__