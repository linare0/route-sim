#include "type.hpp"

bool AdvPktPathCompare::operator()(const AdvPktPath& a,const AdvPktPath& b)const
{
	if(a.src == b.src)
		return a.dest < b.dest;
	else
		return a.src < b.src;
}

AdvPktFactory::AdvPktFactory()
{
	ptr.first = nullptr;
	ptr.second = 0;
}

AdvPktFactory::AdvPktFactory(NodeId MyId)
{
	header.lastHop = MyId;
	ptr.first = nullptr;
	ptr.second = 0;
}

AdvPktFactory::~AdvPktFactory()
{
	if(ptr.first != nullptr)
	free(ptr.first);
}

void AdvPktFactory::addPath(NodeId Src,NodeId Dest)
{
	temp.src = Src;
	temp.dest = Dest;
	paths.insert(temp);
}

void AdvPktFactory::deletePath(NodeId Src,NodeId Dest)
{
	temp.src = Src;
	temp.dest = Dest;
	paths.erase(temp);
}

void AdvPktFactory::clearPath(void)
{
	paths.clear();
}

void AdvPktFactory::setMyId(NodeId MyId)
{
	header.lastHop = MyId;
}

std::pair<void*,size_t> AdvPktFactory::buildPkt(void)
{
	header.magic = MAGIC_ADV;
	header.count = paths.size();
	if(ptr.first != nullptr)
		free(ptr.first);
	ptr.second = sizeof(AdvPktHdr) + (sizeof(AdvPktPath) * paths.size());
	ptr.first = malloc(ptr.second);
	memcpy(ptr.first,&header,sizeof(AdvPktHdr));
	AdvPktPath* pathptr = (AdvPktPath*)((size_t)ptr.first + sizeof(AdvPktHdr));
	uint32_t index = 0;
	for(const auto& ref : paths)
	{
		pathptr[index].src = ref.src;
		pathptr[index].dest = ref.dest;
		index++;
	}
	return ptr;
}

DataPktFactory::DataPktFactory(NodeId MyId) {
	pkt = malloc(MAX_PAYLOAD + sizeof(DataPktHdr));
	pktHdr = (DataPktHdr*) pkt;
	pktPld = (void*) ((size_t) pkt + (sizeof(DataPktHdr)));
	myId = MyId;
	pktHdr->magic = MAGIC_DATA;
	pktHdr->lastHop = MyId;
	pktHdr->src = MyId;
	pktHdr->time = 0;
}

std::pair<void*, size_t> DataPktFactory::buildPkt(const NodeId Dest,
		const uint8_t Ttl, const void* Data, const size_t Count) {
	std::pair<void*, size_t> retval;
	retval.first = pkt;
	retval.second = Count + sizeof(DataPktHdr);
	memcpy(pktPld, Data, Count);
	pktHdr->dest = Dest;
	pktHdr->ttl = Ttl;
	pktHdr->size = Count;
	return retval;
}

