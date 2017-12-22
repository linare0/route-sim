#ifndef TYPE_HPP
#define TYPE_HPP

#include <stdint.h>
#include <set>
#include <string.h>
#include <stdlib.h>
#include <utility>

#define MAGIC_ADV 0x43
#define MAGIC_DATA 0xb4
#define MAX_PAYLOAD 10000
#define MAX_TTL 255

typedef uint8_t NodeId;
typedef uint32_t PathId;
typedef uint8_t Magic;
typedef struct
{
	Magic magic;
	NodeId lastHop;
	uint16_t count;
} __attribute__((__packed__)) AdvPktHdr;

typedef struct
{
	NodeId src;
	NodeId dest;
} __attribute__((__packed__)) AdvPktPath;

class AdvPktPathCompare
{
public:
	bool operator()(const AdvPktPath& a,const AdvPktPath& b)const;
};

class AdvPktFactory
{
private:
	std::set<AdvPktPath,AdvPktPathCompare> paths;
	std::pair<void*,size_t> ptr;
	AdvPktHdr header;
	AdvPktPath temp;
public:
	AdvPktFactory();
	AdvPktFactory(NodeId MyId);
	~AdvPktFactory();
	void addPath(NodeId Src,NodeId Dest);
	void deletePath(NodeId Src,NodeId Dest);
	void clearPath(void);
	void setMyId(NodeId MyId);
	std::pair<void*,size_t> buildPkt(void);
};

typedef struct {
	Magic magic;
	NodeId lastHop;
	NodeId nextHop;
	uint8_t ttl;
	uint8_t time;
	NodeId src;
	NodeId dest;
	uint16_t size;
}__attribute__((__packed__)) DataPktHdr;

class DataPktFactory {
private:
	NodeId myId;
	void* pkt;
	DataPktHdr* pktHdr;
	void* pktPld;
public:
	DataPktFactory(NodeId MyId);
	std::pair<void*, size_t> buildPkt(const NodeId Dest,const NodeId NextHop , const uint8_t Ttl,
			const void* Data, const size_t Count);
};

#endif
