#include "time.hpp"

struct timeval Clock::start;
struct timeval Clock::current;

void Clock::init() {
	gettimeofday(&start, NULL);
}

uint32_t Clock::getCurrent() {
	gettimeofday(&current, NULL);
	return (current.tv_usec - start.tv_usec) / 1000
			+ (current.tv_sec - start.tv_sec) * 1000;
}
