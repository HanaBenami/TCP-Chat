// TCP Chat - Client
// Author: Hana
// Last update: 07/03/2020
/*---------------------------------------------------------------------------*/
#include <iostream>			// std::cout
#include <iostream>			// std::cin.getline()
#include <cstring>			// strlen(), strcmp()
#include <cstdlib>			// exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <pthread.h>		// pthread_create, pthread_join
#include <cassert>			// assert()

#include "sigint_handler.hpp"	// SetSigintHandler(), IsActive(), ShutDown()
#include "tcp_client.hpp"		// PORT, MSG_LEN
#include "chat_settings.hpp"	// TCPSocketClient class
/*---------------------------------------------------------------------------*/
namespace
{
// Global variables
std::atomic<bool> disconnectMsgSent(false);	
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Gets the username from the user.
std::string GetUserNameIMP();

// Creates a thread for sending messages.
pthread_t CreateSendMsgThreadIMP(const TCPSocketClient *socket_);

// Sends a single message to the chat.
ssize_t SendMsgIMP(const TCPSocketClient *socket, const std::string& str_);

// Gets input from the user and send it to the chat.
void InputHandlerIMP(const TCPSocketClient *socket);

// Gets and prints all the messages from the server.
void GetMsgFromServerIMP(const TCPSocketClient *socket_);

} // anonymous namespce
/*---------------------------------------------------------------------------*/
int main()
{
	try 
	{
		SetSigintHandler();
		
		std::string name = GetUserNameIMP();
		
		TCPSocketClient socket(PORT);
		SendMsgIMP(&socket, name);
	
		pthread_t sendThread = CreateSendMsgThreadIMP(&socket);
		
		GetMsgFromServerIMP(&socket);
		
		pthread_cancel(sendThread);
		if (!disconnectMsgSent.load())
		{
			SendMsgIMP(&socket, DISCONNECT_STR);
		}
		
		pthread_mutex_destroy(&lock);
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
std::string GetUserNameIMP()
{
	std::cout << "Please enter a username (up to " << USERNAME_LEN 
			<< " letters):" << std::endl;

	std::string name;
	std::getline(std::cin, name);

	return name;
}
/*---------------------------------------------------------------------------*/
pthread_t CreateSendMsgThreadIMP(const TCPSocketClient *socket_)
{
	typedef void*(*start_routine_t)(void*);
	pthread_t thread_id = 0;
	
	int status = pthread_create(&thread_id, NULL, 
					(start_routine_t)InputHandlerIMP, (void*)socket_);
	if (EXIT_SUCCESS != status)
	{
		throw std::runtime_error("Cannot create thread");
	}
			
	return thread_id;
}
/*---------------------------------------------------------------------------*/
void GetMsgFromServerIMP(const TCPSocketClient *socket_)
{
	assert(NULL != socket_);
	
	while (IsActive())
	{
		char msg[MSG_LEN + 1] = { 0 };
		ssize_t readCounter = recv(socket_->GetFD(), msg, MSG_LEN, 0);
		if (0 < readCounter)
		{
			pthread_mutex_lock(&lock);
			const char* runner = msg;
			while (0 < strlen(runner))
			{
				std::cout << runner << std::endl;
				
				runner += strlen(runner) + 1;
			}
			pthread_mutex_unlock(&lock);
		}
	}
}
/*---------------------------------------------------------------------------*/
ssize_t SendMsgIMP(const TCPSocketClient *socket_, const std::string& str_)
{
	assert(NULL != socket_);
	
	return send(socket_->GetFD(), str_.c_str(), strlen(str_.c_str()), 0);
}
/*---------------------------------------------------------------------------*/
void InputHandlerIMP(const TCPSocketClient *socket_)
{
	assert(NULL != socket_);
	
	std::cout << "\nPlease enter all your messages. "
			<< "To disconnect, send " << DISCONNECT_STR << " at any time.\n" 
			<< std::endl;
	
	char msg[MSG_LEN + 1] = { 0 };
	do 
	{
		pthread_mutex_lock(&lock);
		pthread_mutex_unlock(&lock);
		
		std::cin.getline(msg, MSG_LEN);
		
		pthread_mutex_lock(&lock);
		SendMsgIMP(socket_, msg);
		pthread_mutex_unlock(&lock);
	}
	while (0 != strcmp(DISCONNECT_STR.c_str(), msg));

	disconnectMsgSent = true;
	ShutDown();
}
/*---------------------------------------------------------------------------*/
} // anonymous namespce