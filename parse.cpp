#include "parse.hpp"

void PacketParser::parseAdvPkt(const void* Data,const size_t Count)
{
    if(*(Magic*)Data == MAGIC_ADV)
    {
        AdvPktHdr* pkt = (AdvPktHdr*)Data;
        AdvPktPath* pts = (AdvPktPath*)((size_t)Data + sizeof(AdvPktHdr));
        printf("ADV PACKET: COUNT=%d LASTHOP=%d\n",pkt->count,pkt->lastHop);
        for(uint32_t i = 0;i < pkt->count;i++)
        {
            printf("PATH SRC=%d DEST=%d\n",pts[i].src,pts[i].dest);
        }
        printf("\n");
    }
}

void PacketParser::parse(const void* Data,const size_t Count)
{
    switch(*(Magic*)Data)
    {
    case MAGIC_ADV:
        parseAdvPkt(Data,Count);
        break;
    }
}
