#ifndef __TIMERMICROS_H__
#define __TIMERMICROS_H__
#include "libraries/SDL/include/SDL.h"

//Timer resolution : microseconds
class TimerMicros
{
public:

	TimerMicros();
	~TimerMicros();
	void Start();
	float Read();
	float Stop();
	static Uint64 GetFrequency();

private:

	Uint64 current = 0;
	Uint64 previous = 0;
	bool stopped = false;
	



};
#endif