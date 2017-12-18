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
		else if(buf == "start")
		{
			goto startmain;
			break;
		}
		events.push(tmpEvent);
	}
startmain:
	uint32_t elapsed = 0;
	Time timer = Time();
	while(!events.empty())
	{
		while(events.top().time <= timer.getCurrent())
		{
			switch(events.top().type)
			{
				case VERB_DECLARE:
				{
					auto node = new Node(events.top().args[0],send);
					book.insert(std::make_pair(events.top().args[0],node));
					printf("[%7d]Node %d has declaed.\n",timer.getCurrent(),events.top().args[0]);
					break;
				}
				case VERB_CONNECT:
				{
					auto path = new Path(events.top().args[1],events.top().args[2],&book,events.top().args[3]);
					paths[events.top().args[0]] = path;
					printf("[%7d]Path %d has connected between %d and %d with delay for %d.\n",timer.getCurrent(),events.top().args[0],events.top().args[1],events.top().args[2],events.top().args[3]);
					break;
				}
				case VERB_DISCONNECT:
				{
					delete paths[events.top().args[0]];
					paths.erase(events.top().args[0]);
					printf("[%7d]Path %d has disconnected.\n",timer.getCurrent(),events.top().args[0]);
					break;
				}
				case VERB_END:
				{
					printf("[%7d]Exiting...\n",timer.getCurrent());
					goto endmain;
					break;
				}
			}
			events.pop();
		}
		while(elapsed == 0)
		{
			elapsed = timer.elapsed();
			usleep(800);
		}
		for(const auto& ref: paths)
			ref.second->timeElapsed(elapsed);
		elapsed = 0;
	}
endmain:
	return 0;
}
