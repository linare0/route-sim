#include <queue>
#include <map>
#include <string.h>
#include <stdlib.h>
#include "type.hpp"

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
		Pending(void* Data,size_t Count, unsigned long DueTime,NodeId Dest);
		~Pending();
		bool operator<(Pending* a);
	};
	unsigned long delay;
	unsigned long currentTime;
	NodeId node[2];
	std::map<NodeId,void(*)(void*,size_t)>* bookPtr;
	std::priority_queue<Pending> awaits;
public:
	Path();
	Path(NodeId Node1,NodeId Node2,void* BookPtr,unsigned long Delay);
	void setDelay(unsigned long Delay);
	void setBookPtr(void* BookPtr);
	void connect(NodeId Node1,NodeId Node2);
	void disconnect();
	void transmit(NodeId Src,const void *Data,size_t Count);
	void timeElapsed(unsigned long Elapsed);
};
