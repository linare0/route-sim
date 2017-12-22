#include "main.hpp"

Event::Event()
{
	type = 0;
	time = 0;
	args[0] = 0;
	args[1] = 0;
	args[2] = 0;
	args[3] = 0;
}

Event::Event(uint32_t Type,uint32_t Time,uint32_t Arg1,uint32_t Arg2,uint32_t Arg3,uint32_t Arg4)
{
	type = Type;
	time = Time;
	args[0] = Arg1;
	args[1] = Arg2;
	args[2] = Arg3;
	args[3] = Arg4;
}

bool Event::operator<(const Event a)const
{
	return time > a.time;
}

void send(NodeId src,void* data,size_t count)
{
	for(const auto& ref: paths)
		ref.second->transmit(src,data,count);
}

int main(void)
{
	std::string buf;
	Event tmpEvent;
	while(true)
	{
		std::cin >> buf;
		if(buf == "declare")
		{
			tmpEvent.type = VERB_DECLARE;
			std::cin >> tmpEvent.time >> tmpEvent.args[0];
		}
		else if(buf == "connect")
		{
			tmpEvent.type = VERB_CONNECT;
			std::cin >> tmpEvent.time >> tmpEvent.args[0] >> tmpEvent.args[1] >> tmpEvent.args[2] >> tmpEvent.args[3];
		}
		else if(buf == "disconnect")
		{
			tmpEvent.type = VERB_DISCONNECT;
			std::cin >> tmpEvent.time >> tmpEvent.args[0];
		}
		else if(buf == "end")
		{
			tmpEvent.type = VERB_END;
			std::cin >> tmpEvent.time;
		}
		else if (buf == "data") {
			tmpEvent.type = VERB_DATA;
			std::cin >> tmpEvent.time >> tmpEvent.args[0] >> tmpEvent.args[1]
					>> tmpEvent.args[2];
		}
		else if(buf == "start")
		{
			goto startmain;
			break;
		}
		else
		{
			printf("Parse error: unknown verb: %s",buf);
			exit(-1);
		}
		events.push(tmpEvent);
	}
startmain:
	uint32_t elapsed = 0;
	uint32_t lastElapsed = 0;
	void* nulldata = malloc(10000);
	while(!events.empty())
	{
		while (events.top().time <= Clock::getCurrent())
		{
			switch(events.top().type)
			{
			case VERB_DECLARE:
				{
					auto node = new Node(events.top().args[0],30000,send);
					book.insert(std::make_pair(events.top().args[0],node));
				printf("[%7d]Node %03d has declared.\n",
						(int) Clock::getCurrent(),
						events.top().args[0]);
					break;
				}
			case VERB_CONNECT:
				{
					auto path = new Path(events.top().args[1],events.top().args[2],&book,events.top().args[3]);
					paths[events.top().args[0]] = path;
				printf(
						"[%7d]Path %03d has connected between %03d and %03d with delay for %05d.\n",
						(int) Clock::getCurrent(), events.top().args[0],
						events.top().args[1], events.top().args[2],
						events.top().args[3]);
					break;
				}
			case VERB_DISCONNECT:
				{
					delete paths[events.top().args[0]];
					paths.erase(events.top().args[0]);
				printf("[%7d]Path %03d has disconnected.\n",
						(int) Clock::getCurrent(), events.top().args[0]);
					break;
				}
			case VERB_END:
				{
				printf("[%7d]Exiting...\n", (int) Clock::getCurrent());
					goto endmain;
					break;
				}
			case VERB_DATA: {
				book.find(events.top().args[0])->second->forceSendData(
						events.top().args[1], events.top().args[2], nulldata,
						events.top().args[3]);
				printf(
						"{%7d]Transmitting packet from %03d to %03d with fixed size %04d Bytes");
			}
			}
			events.pop();
		}
#define REALTIME
#ifdef REALTIME
		while(elapsed == 0)
		{
			elapsed = Clock::getCurrent() - lastElapsed;
			lastElapsed = Clock::getCurrent();
			usleep(800);
		}
#else
		elapsed = 1;
#endif
		for(const auto& ref: paths)
			ref.second->timeElapsed(elapsed);
		for(const auto& ref: book)
			ref.second->timeElapsed(elapsed);
		PacketParser::timeElapsed(elapsed);
		elapsed = 0;
	}
endmain:
	for(const auto& ref: paths)
		delete ref.second;
	for(const auto& ref: book)
		delete ref.second;
	return 0;
}
