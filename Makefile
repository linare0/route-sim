CXX=g++
CXXFLAGS=-O0 -g3 -std=c++11 -Wall
LIBS=
OBJS=path.o node.o test.o
PROGRAM=route-sim
all:	$(PROGRAM)
$(PROGRAM):	$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)
clean:
	rm -f *.o *~ $(PROGRAM)
run:	$(PROGRAM)
	./$<
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
