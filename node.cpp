#include "node.hpp"

Node::Node(NodeId MyId,void(*OutPtr)(NodeId,void*,size_t))
{
	myId = MyId;
	transmit = OutPtr;
}

bool Node::Compare::operator()(const Path_& a,const Path_& b)const
{
	if(a.src == b.src)
		return a.dest < b.dest;
	else
		return a.src < b.src;
}

void Node::recieve(void* Data,size_t Count)
{
	switch(*(Magic*)Data)
	{
	case MAGIC_ADV:
		procAdv(Data,Count);
		break;
	default:
		break;
	}
};

void Node::procAdv(void* Data,size_t Count)
{
	const AdvPkt* pkt = (AdvPkt*)Data;
	Path_* inp = (Path_*)((size_t)Data + sizeof(AdvPkt));
	bool changed = false;
	for(uint32_t i = 0;i < pkt->count;i++)
	{
		if(paths.find(inp[i]) == paths.end())
		{
			paths.insert(inp[i]);
			changed = true;
		};
	}
	{
		Path_ temp;
		temp.src = pkt->lastHop;
		temp.dest = myId;
		if(paths.find(temp) == paths.end())
		{
			paths.insert(temp);
			changed = true;
		}
	}
	if(changed)
	{
		AdvPkt* npkt = (AdvPkt*)malloc(sizeof(Path_) * paths.size() + sizeof(AdvPkt));
		npkt->magic = MAGIC_ADV;
		npkt->count = paths.size();
		npkt->lastHop = myId;
		Path_* innp = (Path_*)((size_t)npkt + sizeof(AdvPkt));
		uint32_t index = 0;
		for(const auto& ref : paths)
		{
			innp[index].src = ref.src;
			innp[index].dest = ref.dest;
			index++;
		}
		transmit(myId,npkt,sizeof(Path_) * paths.size() + sizeof(AdvPkt));
	}
}
