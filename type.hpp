#ifndef TYPE_HPP
#define TYPE_HPP

#include <stdint.h>
#include <set>
#include <string.h>
#include <stdlib.h>
#include <utility>

#define MAGIC_ADV 0x436709e0

typedef uint32_t NodeId;
typedef uint32_t PathId;
typedef uint32_t Magic;
typedef struct
{
	Magic magic;
	NodeId lastHop;
	uint32_t count;
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

#endif
