// TCP Chat - Chat user
// Author: Hana
// Last update: 07/03/2020
/*---------------------------------------------------------------------------*/
#ifndef  __CHAT_USER_HPP__
#define  __CHAT_USER_HPP__
/*---------------------------------------------------------------------------*/
#include "uncopyable.hpp"	// Uncopyable class
/*---------------------------------------------------------------------------*/
class ChatUser : private Uncopyable
{
public:
	ChatUser(int client_fd_, const std::string& username_); 
	~ChatUser();
	std::string GetName(size_t len_);
	std::string GetName();
	
	int m_fd;

private:	
	const std::string m_name;
	const size_t m_id;
	const std::string m_color;
	
	static size_t s_counter;
	static const std::string START_COLOR;
	static const char* COLORS[];
	static const std::string RESET_COLOR;
	static const size_t NUM_OF_COLORS;
};	
/*---------------------------------------------------------------------------*/
size_t ChatUser::s_counter = 0;
const std::string ChatUser::START_COLOR = "\033[1m\033[";
const char* ChatUser::COLORS[] = { "31m", "32m", "33m", "34m", "35m", "36m", "37m", 
						"90", "91", "92m", "93m", "94", "95", "96m", "97m" };
const std::string ChatUser::RESET_COLOR = "\033[0m";
const size_t ChatUser::NUM_OF_COLORS = sizeof(COLORS) / sizeof(char*);
/*---------------------------------------------------------------------------*/
ChatUser::ChatUser(int client_fd_, const std::string& username_)
	: m_fd(client_fd_)
	, m_name(username_)
	, m_id(++s_counter)
	, m_color(COLORS[m_id % NUM_OF_COLORS])
{ }

ChatUser::~ChatUser()
{
	close(m_fd);
}

std::string ChatUser::GetName(size_t len_)
{
	std::string name = m_name + " (" + std::to_string(m_id) + ")";	
	name.resize(len_ + 4, ' ');
	name = START_COLOR + m_color + name + RESET_COLOR;
	return name;
}

std::string ChatUser::GetName()
{
	return GetName(m_name.length());
}
/*---------------------------------------------------------------------------*/
#endif // __CHAT_USER_HPP__