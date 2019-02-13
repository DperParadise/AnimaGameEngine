#ifndef _TIMER_CHRONO_H
#define _TIMER_CHRONO_H

#include <chrono>

class TimerChrono
{
public:
	TimerChrono();

	void SetTimeOrigin();
	int ReadInstant();
	void Delay(int time_in_ms);

	
private:
	std::chrono::time_point<std::chrono::steady_clock> delay_time;
	std::chrono::time_point<std::chrono::steady_clock> origin;
	std::chrono::time_point<std::chrono::steady_clock> current;

	bool isNewDelayTime = true;
};



#endif
