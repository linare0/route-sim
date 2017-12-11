#include "node.hpp"
#include "path.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

struct timeval start;

int main(void)
{
	struct timeval ct;
	gettimeofday(&start,NULL);
	for(int i = 0;i<10000;i++)
	{
		gettimeofday(&ct,NULL);
		printf("%c[2K\r", 27);
		printf("%d",(ct.tv_usec - start.tv_usec) / 1000 + (ct.tv_sec - start.tv_sec) * 1000);
		fflush(stdout);
		usleep(1000);
	}
};
