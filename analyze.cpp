#include "analyze.hpp"
#include "time.hpp"

std::vector<Traffic*> Analyzer::traffic;

Traffic::Traffic(NodeId ObservedAt, EInOut InOut, void* Data, size_t Count,
		uint32_t Time) {
	observedAt = ObservedAt;
	inOut = InOut;
	data = malloc(Count);
	count = Count;
	time = Time;
}

Traffic::~Traffic() {
	free(data);
}

bool Traffic::CompareByTime::operator ()(const Traffic& a,
		const Traffic& b) const {
	return a.time < b.time;
}

void Analyzer::registerPacket(NodeId ObservedAt, EInOut InOut, void* Data,
		size_t Count, uint32_t Time) {
	traffic.push_back(new Traffic(ObservedAt, InOut, Data, Count, Clock::getCurrent()));
}

void Analyzer::deinit(void)
{
	for(auto& ref :traffic)
		delete ref;
}
