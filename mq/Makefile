# show how to use Message Queue
#
DEBUG=true

CC = gcc
CXX = g++

SRC = zero_mq.cc datamsg.pb.cc gen_random.cc
C_SRC = zeromq_client.cc gen_random.cc datamsg.pb.cc

# Release should use -O2
ifeq ($(DEBUG),false)
CFLAGS += -DDEBUG -O2
else
CFLAGS += -DCHECK_MEM_LEAK -DDEBUG -g -O0
endif

LDFLAGS = -lzmq -lprotobuf


OBJS = $(SRC:.cc=.o)
COBJS = $(C_SRC:.cc=.o)

all:mqs mqc

mqs:$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

mqc:$(COBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) -lm -pthread

.cc.o:
	$(CXX) $(CFLAGS) -c $< -o $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.o mqs mqc
	find . -name '*.o' -exec rm -rf {} ";"
