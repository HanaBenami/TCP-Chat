// TCP Socket
// Author: Hana
// Last update: 06/03/2020
/*---------------------------------------------------------------------------*/
#ifndef  __TCP_HPP__
#define  __TCP_HPP__
/*---------------------------------------------------------------------------*/
#include <sys/socket.h>		// socket()
#include <netinet/in.h> 	// sockaddr_in
#include <unistd.h>			// close()
#include <fcntl.h>			// fcntl()

#include "uncopyable.hpp"	// Uncopyable class
/*---------------------------------------------------------------------------*/
class TCPSocket : private Uncopyable
{
public:
	TCPSocket(short port_);
	virtual ~TCPSocket();
	int GetFD() const;
	const struct sockaddr_in* GetSockaddr() const;
	const socklen_t* GetSocklen() const;
	
private:
	struct sockaddr_in m_sockaddr;
	socklen_t m_socklen;
	int m_fd;
	int InitSocketFD();
};
/*---------------------------------------------------------------------------*/
TCPSocket::TCPSocket(short port_)
	: m_sockaddr({ AF_INET, htons(port_), INADDR_ANY, 0 })
	, m_socklen(sizeof(struct sockaddr_in))
	, m_fd(InitSocketFD())
{ }
/*---------------------------------------------------------------------------*/
TCPSocket::~TCPSocket()
{
	close(m_fd);
}
/*---------------------------------------------------------------------------*/
int TCPSocket::GetFD() const
{
	return m_fd;
}
/*---------------------------------------------------------------------------*/
const struct sockaddr_in* TCPSocket::GetSockaddr() const
{
	return &m_sockaddr;
}
/*---------------------------------------------------------------------------*/
const socklen_t* TCPSocket::GetSocklen() const
{
	return &m_socklen;
}
/*---------------------------------------------------------------------------*/
int TCPSocket::InitSocketFD()
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == socket_fd)
	{
		throw std::runtime_error("Cannot create socket");
	}

	int dummy = 0;
	int status = setsockopt(socket_fd, SOL_SOCKET, 
			SO_REUSEADDR | SO_REUSEPORT, &dummy, sizeof(dummy));
	if (EXIT_SUCCESS != status)
	{
		close(socket_fd);
		throw std::runtime_error("setsockopt() failed");
	}
	
	return socket_fd;
}
/*---------------------------------------------------------------------------*/
#endif // __TCP_HPP__