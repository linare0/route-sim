#include "node.hpp"

Node::Node(NodeId MyId,void(*OutPtr)(NodeId,void*,size_t))
{
	myId = MyId;
	transmit = OutPtr;
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
	const AdvPktHdr* pkt = (AdvPktHdr*)Data;
	AdvPktPath* inp = (AdvPktPath*)((size_t)Data + sizeof(AdvPktHdr));
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
		AdvPktPath temp;
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
		AdvPktHdr* npkt = (AdvPktHdr*)malloc(sizeof(AdvPktPath) * paths.size() + sizeof(AdvPktHdr));
		npkt->magic = MAGIC_ADV;
		npkt->count = paths.size();
		npkt->lastHop = myId;
		AdvPktPath* innp = (AdvPktPath*)((size_t)npkt + sizeof(AdvPktHdr));
		uint32_t index = 0;
		for(const auto& ref : paths)
		{
			innp[index].src = ref.src;
			innp[index].dest = ref.dest;
			index++;
		}
		transmit(myId,npkt,sizeof(AdvPktPath) * paths.size() + sizeof(AdvPktHdr));
	}
}
