#ifndef PARSE_HPP
#define PARSE_HPP

#include "type.hpp"

class PacketParser
{
private:
    PacketParser();
    static void parseAdvPkt(const void* Data,const size_t Count);
public:
    static void parse(const void* Data,const size_t Count);
};

#endif
