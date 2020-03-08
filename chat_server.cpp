// TCP Chat - Server
// Author: Hana
// Last update: 07/03/2020
/*---------------------------------------------------------------------------*/
#include <iostream>			// std::cout
#include <cstring>			// strlen(), strcmp()
#include <cstdlib>			// exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <vector>			// std::vector
#include <cassert>			// assert()

#include "sigint_handler.hpp"	// SetSigintHandler(), IsActive(), ShutDown()
#include "tcp_server.hpp"		// PORT, MSG_CAPACITY, TCPSocketServer
#include "chat_settings.hpp"	// TCPSocketServer class
#include "chat_user.hpp"		// ChatUser class
/*---------------------------------------------------------------------------*/
namespace
{
// Accepts requests for connection and add the file desctiptors of the 
// new connections to the vector.
void AcceptNewConnectionsIMP(const TCPSocketServer* socket_, 
		std::vector<ChatUser*> *users_);
		
// Gets and ptints all the messagesfrom the open connections.
// If a disconnected string has been sent, closes the relevant connection.
void ReadMessagesIMP(std::vector<ChatUser*> *users_);

// Closes all the connections and clears the vector.
void CloseAllTheConnectionsIMP(std::vector<ChatUser*> *users_);

// Creates formatted messages. 
std::string CreateMsgIMP(const std::string& user_, const std::string& msg_);
std::string ConnectMsgIMP(const std::string& user_);
std::string DisconnectMsgIMP(const std::string& user_);

// Sends a message to all the chat users.
void BroadCastIMP(const std::vector<ChatUser*> *users_, 
		const std::string& msg_);

} // anonymous namespace
/*---------------------------------------------------------------------------*/
int main()
{
	try
	{
		SetSigintHandler();
		
		TCPSocketServer socket(PORT, MAX_REQUESTS);

		std::vector<ChatUser*> users;
		while (IsActive())
		{
			AcceptNewConnectionsIMP(&socket, &users);
			ReadMessagesIMP(&users);
		}
		
		CloseAllTheConnectionsIMP(&users);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}	
	
	return EXIT_SUCCESS;
}
/*---------------------------------------------------------------------------*/
namespace
{
/*---------------------------------------------------------------------------*/
void AcceptNewConnectionsIMP(const TCPSocketServer* socket_, 
		std::vector<ChatUser*> *users_)
{
	assert(NULL != socket_);
	assert(NULL != users_);
	
	int connection_fd = accept(socket_->GetFD(), 
			(struct sockaddr*)socket_->GetSockaddr(), 
			(socklen_t*)socket_->GetSocklen());
			
	while (0 < connection_fd)
	{
		char username[USERNAME_LEN + 1] = { 0 };
		recv(connection_fd, username, USERNAME_LEN, 0);
		fcntl(connection_fd, F_SETFL, O_NONBLOCK);
		
		ChatUser* newChatUser = new ChatUser(connection_fd, username);
		users_->push_back(newChatUser);
		
		BroadCastIMP(users_, ConnectMsgIMP(newChatUser->GetName()));
		
		connection_fd = accept(socket_->GetFD(), 
			(struct sockaddr*)socket_->GetSockaddr(), 
			(socklen_t*)socket_->GetSocklen());
	}
}
/*---------------------------------------------------------------------------*/
void ReadMessagesIMP(std::vector<ChatUser*> *users_)
{
	assert(NULL != users_);
	
	for (std::vector<ChatUser*>::iterator iter = users_->begin()
		; iter < users_->end()
		; ++iter)
	{
		char msg[MSG_LEN] = { 0 };
		ssize_t readCounter = recv((*iter)->m_fd, msg, MSG_LEN, 0);
		if (0 < readCounter)
		{
			const char* runner = msg;
			while (0 < strlen(runner))
			{
				std::string newMsg;					
				if (0 == strcmp(DISCONNECT_STR.c_str(), runner))
				{
					newMsg = DisconnectMsgIMP((*iter)->GetName());
					
					delete *iter;
					users_->erase(iter);
					--iter;
				}
				else
				{
					newMsg = CreateMsgIMP(
							(*iter)->GetName(USERNAME_LEN + 2), runner);
				}
				BroadCastIMP(users_, newMsg);
				
				runner += strlen(runner) + 1;
			}
		}
	}
}
/*---------------------------------------------------------------------------*/
void CloseAllTheConnectionsIMP(std::vector<ChatUser*> *users_)
{
	assert(NULL != users_);
	
	for (std::vector<ChatUser*>::iterator iter = users_->begin()
		; iter < users_->end()
		; ++iter)
	{
		delete *iter;
	}
	
	users_->clear();
}
/*---------------------------------------------------------------------------*/
std::string CreateMsgIMP(const std::string& user_, const std::string& msg_)
{
	return (user_ + ": " + msg_);
}
/*---------------------------------------------------------------------------*/
std::string ConnectMsgIMP(const std::string& user_)
{
	return ("\n" + user_ + " has join the chat!\n");
}
/*---------------------------------------------------------------------------*/
std::string DisconnectMsgIMP(const std::string& user_)
{
	return ("\n" + user_ + " has disconnected.\n");
}
/*---------------------------------------------------------------------------*/
void BroadCastIMP(const std::vector<ChatUser*>* users_, 
		const std::string& msg_)
{
	assert(NULL != users_);
	
	std::cout << msg_ << std::endl;
	
	for (std::vector<ChatUser*>::const_iterator iter = users_->begin()
		; iter < users_->end()
		; ++iter)
	{
		send((*iter)->m_fd, msg_.c_str(), strlen(msg_.c_str()), 0);
	}
}
/*---------------------------------------------------------------------------*/
} // anonymous namespce