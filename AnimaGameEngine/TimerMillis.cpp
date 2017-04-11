#include "TimerMillis.h"

TimerMillis::TimerMillis() : current(0), previous(0), stopped(false) {}
TimerMillis::~TimerMillis() {}

void TimerMillis::Start()
{
	previous = SDL_GetTicks();
	current = previous;
	stopped = false;
}
//returns time in milliseconds
Uint32 TimerMillis::Read()
{
	Uint32 ret;
	
	if (!stopped)
	{
		current = SDL_GetTicks();
		ret = current - previous;
	}

	else
		ret = current;

	return ret;
}

Uint32 TimerMillis::Stop()
{
	Uint32 ret;
	stopped = true;
	ret = current;

	return ret;
	
}