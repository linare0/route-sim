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
	void calcRoute(void);
	void procAdv(void* Data,size_t Count);
public:
	Node(NodeId MyId,void(*OutPtr)(NodeId,void*,size_t));
	void recieve(void* Data,size_t Count);
};

#endif
