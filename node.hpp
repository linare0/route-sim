#ifndef NODE_HPP
#define NODE_HPP

#include "type.hpp"
#include "parse.hpp"
#include "analyze.hpp"
#include "time.hpp"
#include <set>
#include <algorithm>
#include <map>

class Node
{
private:
	std::set<AdvPktPath,AdvPktPathCompare> paths;
	std::map<NodeId,NodeId> nextHop;
	void (*transmit_raw)(NodeId, void*, size_t);
	void (*capture)(NodeId ObservedAt, EInOut InOut, void* Data, size_t Count);
	NodeId myId;
	unsigned long advInterval;
	unsigned long advertized;
	void transmit(void* Data, size_t Count);
	void calcRoute(void);
	void procAdv(void* Data,size_t Count);
	DataPktFactory dataPktFactory;
public:
	Node(NodeId MyId, unsigned long Interval,
			void (*OutPtr)(NodeId, void*, size_t));
	~Node();
	void setAdvInterval(unsigned long Interval);
	void timeElapsed(unsigned long Elapsed);
	void forceAdvertize();
	void forceSendData(NodeId Dest, uint8_t Tll, void* Data, size_t Count);
	void recieve(void* Data,size_t Count);
};

#endif
