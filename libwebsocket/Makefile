# show how to use WEBSOCKET
#
DEBUG=true

CC = gcc
CXX = g++

SRC = server.cc

C_SRC = client.cc

# Release should use -O2
ifeq ($(DEBUG),false)
CXXFLAGS += -DDEBUG -O2
else
CXXFLAGS += -DCHECK_MEM_LEAK -DDEBUG -g -O0
endif

LDFLAGS = -lwebsockets


OBJS = $(SRC:.cc=.o)
COBJS = $(C_SRC:.cc=.o)

all:webs webc

webs:$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

webc:$(COBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CXX) $(CXXLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.o webs webc
	find . -name '*.o' -exec rm -rf {} ";"
