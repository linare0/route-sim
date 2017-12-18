#ifndef PARSE_HPP
#define PARSE_HPP

#include <stdio.h>
#include "type.hpp"
class PacketParser
{
private:
    PacketParser();
    static size_t sumBytes;
    static int sumCount;
    static unsigned long currentTime;
    static void parseAdvPkt(const void* Data,const size_t Count);
public:
    static void parse(const void* Data,const size_t Count,NodeId Id);
    static void timeElapsed(unsigned long Elapsed);
};

#endif
