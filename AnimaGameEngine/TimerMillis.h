#ifndef __TIMERMILLIS_H__
#define __TIMERMILLIS_H__
#include "libraries/sdl/include/SDL.h"


class TimerMillis
 {
 public:

	 TimerMillis();
	 ~TimerMillis();
	 void Start();
	 Uint32 Read();
	 Uint32 Stop();

 private:

	 Uint32 current = 0;
	 Uint32 previous = 0;
	 bool stopped = false;
};


#endif