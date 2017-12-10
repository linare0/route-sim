#include <stdint.h>

#ifndef TYPE_HPP
#define TYPE_HPP

#define MAGIC_ADV 0x436709e0

typedef uint32_t NodeId;
typedef uint32_t Magic;
typedef struct
{
	Magic magic;
	NodeId lastHop;
	uint32_t count;
} __attribute__((__packed__)) AdvPkt;

#endif
