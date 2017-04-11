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
double TimerMicros::Read()
{
	double ret;

	if (!stopped)
	{
		current = SDL_GetPerformanceCounter();
		ret = (double)(current - previous);
	}

	else
		ret = (double)current;

	ret = (double)(ret * 1000 / GetFrequency());
	
	return ret;
}

double TimerMicros::Stop()
{
	double ret;
	stopped = true;
	ret = (double)current;

	ret = (double)(ret * 1000 / GetFrequency());

	return ret;

}

Uint64 TimerMicros::GetFrequency() 
{
	static Uint64 frequency = SDL_GetPerformanceFrequency();
	return frequency;
}