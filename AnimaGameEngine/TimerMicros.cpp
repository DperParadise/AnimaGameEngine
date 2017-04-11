#include "TimerMicros.h"

TimerMicros::TimerMicros() : current(0), previous(0), stopped(false) {}

TimerMicros::~TimerMicros() {}

void TimerMicros::Start()
{
	previous = SDL_GetPerformanceCounter();
	current = previous;
	stopped = false;
}
//returns time in milliseconds
float TimerMicros::Read()
{
	float ret;

	if (!stopped)
	{
		current = SDL_GetPerformanceCounter();
		ret = (float)(current - previous);
	}

	else
		ret = (float)current;

	ret = (float)(ret * 1000 / GetFrequency());
	
	return ret;
}

float TimerMicros::Stop()
{
	float ret;
	stopped = true;
	ret = (float)current;

	ret = (float)(ret * 1000 / GetFrequency());

	return ret;

}

Uint64 TimerMicros::GetFrequency() 
{
	static Uint64 frequency = SDL_GetPerformanceFrequency();
	return frequency;
}