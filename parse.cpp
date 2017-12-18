#include "parse.hpp"

size_t PacketParser::sumBytes;
int PacketParser::sumCount;
unsigned long PacketParser::currentTime;

void PacketParser::parseAdvPkt(const void* Data,const size_t Count)
{
    if(*(Magic*)Data == MAGIC_ADV)
    {
        AdvPktHdr* pkt = (AdvPktHdr*)Data;
        AdvPktPath* pts = (AdvPktPath*)((size_t)Data + sizeof(AdvPktHdr));
        printf("ADV PACKET: COUNT=%d LASTHOP=%d SIZE=%dB\n",pkt->count,pkt->lastHop,(int)Count);
        for(uint32_t i = 0;i < pkt->count;i++)
        {
            printf("PATH SRC=%d DEST=%d\n",pts[i].src,pts[i].dest);
        }
        printf("\n");
    }
}

void PacketParser::parse(const void* Data,const size_t Count,NodeId Id)
{
    sumCount++;
    sumBytes += Count;
    printf("[%7d]Packet recieved at %03d Count=%05d Bytes=%07d\n",(int)currentTime,(int)Id,sumCount,(int)sumBytes);
    switch(*(Magic*)Data)
    {
    case MAGIC_ADV:
        parseAdvPkt(Data,Count);
        break;
    }
}

void PacketParser::timeElapsed(unsigned long Elapsed)
{
    currentTime += Elapsed;
}
