#ifndef MAIN_HPP
#define MAIN_HPP

#include "node.hpp"
#include "path.hpp"
#include "parse.hpp"
#include "analyze.hpp"
#include "time.hpp"
#include <stdint.h>
#include <queue>
#include <string>
#include <iostream>

#define VERB_DECLARE 1
#define VERB_CONNECT 2
#define VERB_DISCONNECT 3
#define VERB_END 4
#define VERB_DATA 5

class Event
{
public:
	Event();
	Event(uint32_t Type,uint32_t Time,uint32_t Arg1,uint32_t Arg2,uint32_t Arg3,uint32_t Arg4);
	bool operator<(const Event a)const;
	uint32_t type;
	uint32_t time;
	uint32_t args[4];
};

void parse(void* data,size_t count);
void send(NodeId src,void* data,size_t count);

std::map<PathId,Path*> paths;
std::map<NodeId,Node*> book;
std::priority_queue<Event> events;

#endif
