#ifndef NODE_HPP
#define NODE_HPP

#include "type.hpp"
#include "parse.hpp"
#include <set>
#include <algorithm>
#include <map>

class Node
{
private:
	std::set<AdvPktPath,AdvPktPathCompare> paths;
	std::map<NodeId,NodeId> nextHop;
	void(*transmit)(NodeId,void*,size_t);
	NodeId myId;
	unsigned long advInterval;
	unsigned long advertized;
	unsigned long recvCount,recvByte,sentCount,sentByte;
	void calcRoute(void);
	void procAdv(void* Data,size_t Count);
public:
	Node(NodeId MyId,unsigned long Interval,void(*OutPtr)(NodeId,void*,size_t));
	~Node();
	void setAdvInterval(unsigned long Interval);
	void timeElapsed(unsigned long Elapsed);
	void recieve(void* Data,size_t Count);
};

#endif
