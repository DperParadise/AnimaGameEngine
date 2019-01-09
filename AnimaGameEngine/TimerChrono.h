#ifndef _TIMER_CHRONO_H
#define _TIMER_CHRONO_H

#include <chrono>

class TimerChrono
{
public:
	TimerChrono();

	void Start();
	int Read();
	void Delay(int time_in_ms);

private:
	std::chrono::time_point<std::chrono::steady_clock> prev_time;
	std::chrono::time_point<std::chrono::steady_clock> curr_time;

	std::chrono::time_point<std::chrono::steady_clock> delay_time;
};



#endif
