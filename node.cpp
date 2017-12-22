#include "node.hpp"

Node::Node(NodeId MyId, unsigned long Interval,
		void (*OutPtr)(NodeId, void*, size_t)) :
		dataPktFactory(MyId)
{
	advInterval = Interval;
	advertized = 0;
	myId = MyId;
	transmit_raw = OutPtr;
	forceAdvertize();
}

Node::~Node() {

}

void Node::transmit(void* Data, size_t Count) {
	Analyzer::registerPacket(myId, OUT, Data, Count, Clock::getCurrent());
	transmit_raw(myId, Data, Count);
}

void Node::recieve(void* Data,size_t Count)
{
	Analyzer::registerPacket(myId, IN, Data, Count, Clock::getCurrent());
	PacketParser::parse(Data,Count,myId);
	switch(*(Magic*)Data)
	{
	case MAGIC_ADV:
		procAdv(Data,Count);
		break;
	default:
		break;
	}
};

void Node::setAdvInterval(unsigned long Interval)
{
	advInterval = Interval;
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
		forceAdvertize();
}

void Node::timeElapsed(unsigned long Elapsed)
{
	advertized += Elapsed;
	if(advertized >= advInterval)
	{
		advertized = 0;
		forceAdvertize();
	}
}

void Node::forceAdvertize(void) {
	AdvPktHdr* npkt = (AdvPktHdr*) malloc(
			sizeof(AdvPktPath) * paths.size() + sizeof(AdvPktHdr));
	npkt->magic = MAGIC_ADV;
	npkt->count = paths.size();
	npkt->lastHop = myId;
	AdvPktPath* innp = (AdvPktPath*) ((size_t) npkt + sizeof(AdvPktHdr));
	uint32_t index = 0;
	for (const auto& ref : paths) {
		innp[index].src = ref.src;
		innp[index].dest = ref.dest;
		index++;
	}
	transmit(npkt, sizeof(AdvPktPath) * paths.size() + sizeof(AdvPktHdr));
	free(npkt);
}

void Node::forceSendData(NodeId Dest, uint8_t Ttl, void* Data, size_t Count) {
	auto pkt = dataPktFactory.buildPkt(Dest, Ttl, Data, Count);
	transmit(pkt.first, pkt.second);
}

