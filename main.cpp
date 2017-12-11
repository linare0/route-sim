#include "main.hpp"

Time::Time()
{
	gettimeofday(&start,NULL);
	lastElapse = 0;
}

uint32_t Time::getCurrent()
{
	gettimeofday(&current,NULL);
	return (current.tv_usec - start.tv_usec) / 1000 + (current.tv_sec - start.tv_sec) * 1000;
}

uint32_t Time::elapsed()
{
	uint32_t retval = getCurrent() - lastElapse;
	lastElapse = getCurrent();
	return retval;
}

void parse(void* data,size_t count)
{
	switch(*(Magic*)data)
	{
	case MAGIC_ADV:
		AdvPktHdr* pkt = (AdvPktHdr*)data;
		AdvPktPath* pts = (AdvPktPath*)((size_t)data + sizeof(AdvPktHdr));
		printf("ADV PACKET: COUNT=%d LASTHOP=%d\n",pkt->count,pkt->lastHop);
		for(uint32_t i = 0;i < pkt->count;i++)
		{
			printf("PATH SRC=%d DEST=%d\n",pts[i].src,pts[i].dest);
		}
		printf("\n");
		break;
	}
}

void send(NodeId src,void* data,size_t count)
{
	for(const auto& ref: paths)
		ref->transmit(src,data,count);
}

int main(void)
{
	book.insert(std::make_pair(2,parse));
	Path var2 = Path(1,2,&book,1000);
	paths.push_back(&var2);
	Node var = Node(1,send);
	AdvPktFactory factory = AdvPktFactory(11);
	factory.addPath(2,3);
	auto made = factory.buildPkt();
	var.recieve(made.first,made.second);
	Time timer = Time();
	while(timer.getCurrent() < 5000)
	{
		var2.timeElapsed(timer.elapsed());
		fflush(stdout);
		usleep(1000);
	}
};
