#ifndef ANALYZE_HPP_
#define ANALYZE_HPP_

#include "type.hpp"
#include <vector>
#include <algorithm>

enum EInOut {
	IN, OUT
};

class Traffic {
private:

public:
	NodeId observedAt;
	EInOut inOut;
	void* data;
	size_t count;
	uint32_t time;
	Traffic(NodeId ObservedAt, EInOut InOut, void* Data, size_t Count,
			uint32_t Time);
	~Traffic();
	class CompareByTime {
	public:
		bool operator()(const Traffic& a, const Traffic& b) const;
	};
};

class Analyzer {
private:
	static std::vector<Traffic*> traffic;
public:
	static void init(void);
	static void deinit(void);
	static void registerPacket(NodeId ObservedAt, EInOut InOut, void* Data,
			size_t Count, uint32_t Time);
};

#endif
