#include "path.hpp"

bool Path::Pending::Compare::operator()(const Pending* a,const Pending* b)
const{
	return a->dueTime > b->dueTime;
}

Path::Pending::Pending(void* Data, size_t Count, unsigned long DueTime, NodeId Dest)
{
	data = malloc(Count);
	memcpy(data,Data,Count);
	count = Count;
	dueTime = DueTime;
	dest = Dest;
}

Path::Pending::~Pending()
{
	free(data);
}

Path::Path()
{
	delay = 0;
	currentTime = 0;
	node[0] = 0;
	node[1] = 0;
	bookPtr = NULL;
}

Path::Path(NodeId Node1, NodeId Node2,std::map<NodeId,Node*>* BookPtr, unsigned long Delay)
{
	delay = Delay;
	currentTime = 0;
	node[0] = Node1;
	node[1] = Node2;
	bookPtr = BookPtr;
}

void Path::setDelay(unsigned long Delay)
{
	delay = Delay;
}

void Path::setBookPtr(std::map<NodeId,Node*>* BookPtr)
{
	bookPtr = BookPtr;
}

void Path::connect(NodeId Node1, NodeId Node2)
{
	node[0] = Node1;
	node[1] = Node2;
}

void Path::disconnect()
{
	node[0] = 0;
	node[1] = 0;
}

void Path::transmit(NodeId Src, void *Data, size_t Count)
{
	if(Src == node[1])
	{
		auto var = new Pending(Data,Count,currentTime + delay,node[0]);
		awaits.push(var);
	}
	else if(Src == node[0])
	{
		auto var = new Pending(Data,Count,currentTime + delay,node[1]);
		awaits.push(var);
	}
}

void Path::timeElapsed(unsigned long Elapsed)
{
	currentTime += Elapsed;
	while(!awaits.empty()&& awaits.top()->dueTime <= currentTime)
	{
		if(bookPtr->find(awaits.top()->dest) != bookPtr->end() && bookPtr->find(awaits.top()->dest) != bookPtr->end())
			(bookPtr->find(awaits.top()->dest)->second)->recieve(awaits.top()->data,awaits.top()->count);
		delete awaits.top();
		awaits.pop();
	}
}
