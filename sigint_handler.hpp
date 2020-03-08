// Activity flag SINGINT Handler
// Author: Hana
// Last update: 07/03/2020
/*---------------------------------------------------------------------------*/
#ifndef  __SIGINT_HANDLER_HPP__
#define  __SIGINT_HANDLER_HPP__
/*---------------------------------------------------------------------------*/
#include <signal.h>			// sigaction()
#include <atomic>			// std::atomic<bool>
/*---------------------------------------------------------------------------*/
namespace
{
/*---------------------------------------------------------------------------*/
std::atomic<bool> activeFlag(true);	

// Sets SIGINT handler to the following function
void SetSigintHandler();

// Changes activeFlag to false.
void ShutDown();

// Returns the value of activeFlag
bool IsActive();
/*---------------------------------------------------------------------------*/
void SetSigintHandler()
{
	struct sigaction sigact;
	sigact.sa_handler = (void(*)(int))&ShutDown;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	
	int status = sigaction(SIGINT, &sigact, NULL);
	if (EXIT_SUCCESS != status)
	{
		throw std::runtime_error("Cannot set signal handler");
	}
}
/*---------------------------------------------------------------------------*/
void ShutDown()
{
	activeFlag = false;	
}
/*---------------------------------------------------------------------------*/
bool IsActive()
{
	return activeFlag.load();
}
/*---------------------------------------------------------------------------*/
} // Anonymous namespace
/*---------------------------------------------------------------------------*/
#endif // __SIGINT_HANDLER_HPP__