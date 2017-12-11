#ifndef PATH_HPP
#define PATH_HPP

#include <queue>
#include <map>
#include <functional>
#include <string.h>
#include <stdlib.h>
#include "type.hpp"
#include "node.hpp"

class Path
{
private:
	class Pending
	{
	public:
		unsigned long dueTime;
		void* data;
		size_t count;
		NodeId dest;
		Pending(void* Data, size_t Count,  unsigned long DueTime, NodeId Dest);
		~Pending();
		class Compare
		{
		public:
			bool operator()(const Pending* a,const Pending* b)const;
		};
	};
	unsigned long delay;
	unsigned long currentTime;
	NodeId node[2];
	std::map<NodeId,Node*>* bookPtr;
	std::priority_queue<Pending*,std::vector<Pending*>,Pending::Compare> awaits;
public:
	Path();
	Path(NodeId Node1, NodeId Node2,std::map<NodeId,Node*>* BookPtr, unsigned long Delay);
	void setDelay(unsigned long Delay);
	void setBookPtr(std::map<NodeId,Node*>* BookPtr);
	void connect(NodeId Node1, NodeId Node2);
	void disconnect();
	void transmit(NodeId Src, void *Data, size_t Count);
	void timeElapsed(unsigned long Elapsed);
};

#endif
