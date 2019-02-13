#include "TimerChrono.h"

using namespace std::chrono;

TimerChrono::TimerChrono(){}

void TimerChrono::SetTimeOrigin()
{
	origin = high_resolution_clock::now();
}

int TimerChrono::ReadInstant()
{
	current = high_resolution_clock::now();
	return duration_cast<milliseconds>(current - origin).count();
}

void TimerChrono::Delay(int time_in_ms)
{
	if (isNewDelayTime)
	{
		delay_time = high_resolution_clock::now() + milliseconds(time_in_ms);
		isNewDelayTime = false;
	}

	while (high_resolution_clock::now() < delay_time)
	{	
		//wait
	}
	isNewDelayTime = true;
}


