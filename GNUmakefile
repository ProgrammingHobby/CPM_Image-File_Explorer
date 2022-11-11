INCS            ?= $(shell wx-config --cxxflags)
LIBS            ?= $(shell wx-config --libs)
OBJS            := $(patsubst %.cpp, %.o, $(wildcard CIFE/Sources/*.cpp))
PROG            := cife
CLEAN           := $(PROG) $(OBJS)

CXXFLAGS        ?= -O3 -Wall
CXXFLAGS        += $(INCS)
LDFLAGS		?=  -no-pie -s

.PHONY:         all clean

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)

clean:
	$(RM) $(CLEAN)
