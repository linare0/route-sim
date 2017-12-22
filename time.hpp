#ifndef TIME_HPP
#define TIME_HPP

#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

class Clock {
private:
	static struct timeval start;
	static struct timeval current;
	Clock();
public:
	static void init();
	static uint32_t getCurrent();
};

#endif
