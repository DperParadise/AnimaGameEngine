#include "TimerChrono.h"

using namespace std::chrono;

TimerChrono::TimerChrono(){}

void TimerChrono::Start()
{
	prev_time = steady_clock::now();
	curr_time = prev_time;
}

int TimerChrono::Read()
{
	curr_time = high_resolution_clock::now();
	int elapsed = duration_cast<milliseconds>(curr_time - prev_time).count();
	prev_time = curr_time;

	return elapsed;
	
}

void TimerChrono::Delay(int time_in_ms)
{
	delay_time = high_resolution_clock::now() + milliseconds(time_in_ms);

	while (high_resolution_clock::now() < delay_time)
	{
		//wait	
	}

}
