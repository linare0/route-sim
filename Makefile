CXX=g++
CXXFLAGS=-O2 -std=c++11 -Wall
LIBS=
OBJS=parse.o path.o node.o type.o main.o
PROGRAM=route-sim
all:	$(PROGRAM)
$(PROGRAM):	$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)
clean:
	rm -f *.o *~ $(PROGRAM)
run:	$(PROGRAM)
	./$<
rebuild:
	make clean
	make all
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
