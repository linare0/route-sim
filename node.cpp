#include "node.hpp"

void parse(void* data,size_t count)
{
	switch(*(Magic*)data)
	{
	case MAGIC_ADV:
		AdvPktHdr* pkt = (AdvPktHdr*)data;
		AdvPktPath* pts = (AdvPktPath*)((size_t)data + sizeof(AdvPktHdr));
		printf("ADV PACKET: COUNT=%d LASTHOP=%d\n",pkt->count,pkt->lastHop);
		for(uint32_t i = 0;i < pkt->count;i++)
		{
			printf("PATH SRC=%d DEST=%d\n",pts[i].src,pts[i].dest);
		}
		printf("\n");
		break;
	}
}

Node::Node(NodeId MyId,void(*OutPtr)(NodeId,void*,size_t))
{
	myId = MyId;
	transmit = OutPtr;
	AdvPktHdr pkt;
	pkt.magic = MAGIC_ADV;
	pkt.lastHop = myId;
	pkt.count = 0;
	transmit(myId,&pkt,sizeof(AdvPktHdr));
}

void Node::recieve(void* Data,size_t Count)
{
	parse(Data,Count);
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
