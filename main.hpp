#include "node.hpp"
#include "path.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>

class Time
{
private:
	struct timeval start;
	struct timeval current;
	uint32_t lastElapse;
public:
	Time();
	uint32_t getCurrent();
	uint32_t elapsed();
};

void parse(void* data,size_t count);
void send(NodeId src,void* data,size_t count);

std::vector<Path*> paths;
std::map<NodeId,void(*)(void*,size_t)> book;
